// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ScalingScanlineProcessingBlock.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/core/proc/legacy/SeparableFiltersResampler.h>
#include <spectrum/core/proc/legacy/Sharpener.h>
#include <spectrum/image/Scanline.h>

#include <cmath>
#include <memory>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

//
// ScalingBlockImpl
//

class ScalingBlockImpl : public ScanlineProcessingBlock {
 protected:
  const image::pixel::Specification _pixelSpecification;
  const image::Size inputSize;
  const image::Size outputSize;

  const float scalingX;
  const float scalingY;
  const float invScalingX;
  const float invScalingY;

  std::vector<std::unique_ptr<image::Scanline>> input = {};
  std::size_t nextLineToRelease = 0;
  std::size_t outputScanline = 0;

 public:
  ScalingBlockImpl(
      const image::pixel::Specification& pixelSpecification,
      const image::Size& inputSize,
      const image::Size& outputSize);
  ~ScalingBlockImpl() override = default;
  void consume(std::unique_ptr<image::Scanline> scanline) override;
};

ScalingBlockImpl::ScalingBlockImpl(
    const image::pixel::Specification& pixelSpecification,
    const image::Size& inputSize,
    const image::Size& outputSize)
    : _pixelSpecification(pixelSpecification),
      inputSize(inputSize),
      outputSize(outputSize),
      scalingX((float)outputSize.width / (float)inputSize.width),
      scalingY((float)outputSize.height / (float)inputSize.height),
      invScalingX(1.0f / scalingX),
      invScalingY(1.0f / scalingY) {
  SPECTRUM_ENFORCE_IF_NOT(scalingX > 0);
  SPECTRUM_ENFORCE_IF_NOT(scalingY > 0);
}

void ScalingBlockImpl::consume(std::unique_ptr<image::Scanline> scanline) {
  SPECTRUM_ENFORCE_IF_NOT(scanline->specification() == _pixelSpecification);
  input.push_back(std::move(scanline));
}

//
// Magic kernel
//

class MagicKernelScalingBlockImpl : public ScalingBlockImpl {
 private:
  // for magic kernel only
  std::vector<std::unique_ptr<image::Scanline>> magicOutput = {};
  void runMagicKernel();
  std::unique_ptr<image::Scanline> produceMagicKernel();

 public:
  MagicKernelScalingBlockImpl(
      const image::pixel::Specification& pixelSpecification,
      const image::Size& inputSize,
      const image::Size& outputSize);
  ~MagicKernelScalingBlockImpl() override = default;
  std::unique_ptr<image::Scanline> produce() override;
};

MagicKernelScalingBlockImpl::MagicKernelScalingBlockImpl(
    const image::pixel::Specification& pixelSpecification,
    const image::Size& inputSize,
    const image::Size& outputSize)
    : ScalingBlockImpl(pixelSpecification, inputSize, outputSize) {}

void MagicKernelScalingBlockImpl::runMagicKernel() {
  const auto numComponents = _pixelSpecification.bytesPerPixel;
  const auto stride = outputSize.width * numComponents;

  std::unique_ptr<std::uint8_t[]> lineBuffer(new std::uint8_t[stride]);

  legacy::SeparableFiltersResampler magicResampler(
      inputSize.width,
      inputSize.height,
      outputSize.width,
      outputSize.height,
      numComponents);

  legacy::Sharpener magicSharpener(
      outputSize.width, outputSize.height, numComponents, lineBuffer.get());

  // run
  const std::size_t inputSize = input.size();
  while (nextLineToRelease < inputSize) {
    // input -> resampler
    SPECTRUM_ENFORCE_IF_NOT(input[nextLineToRelease]);
    std::uint8_t* buffer =
        reinterpret_cast<std::uint8_t*>(input[nextLineToRelease]->data());
    SPECTRUM_ENFORCE_IF_NOT(buffer);
    magicResampler.putLine(buffer);

    // resampler -> sharpener
    // elements of `pResampledRow` are Q21.11 fixed-point numbers
    while (const int32_t* pResampledRow = magicResampler.getLine()) {
      magicSharpener.putLine(pResampledRow);

      // sharpener -> output
      while (magicSharpener.getLine(lineBuffer.get())) {
        auto scanline = std::make_unique<image::Scanline>(
            _pixelSpecification, outputSize.width);

        SPECTRUM_ENFORCE_IF_NOT(stride == scanline->sizeBytes());
        SPECTRUM_ENFORCE_IF_NOT(scanline && scanline->data());
        SPECTRUM_ENFORCE_IF_NOT(lineBuffer.get());

        memcpy(scanline->data(), lineBuffer.get(), stride);

        magicOutput.push_back(std::move(scanline));
      }
    }

    // free processed input
    input[nextLineToRelease].release();
    nextLineToRelease++;
  }
}

std::unique_ptr<image::Scanline> MagicKernelScalingBlockImpl::produce() {
  if (input.size() == inputSize.height) {
    // TODO T21712884: stream scanlines instead of processing of all at once
    runMagicKernel();
    input.clear();
  }

  if (outputScanline < magicOutput.size()) {
    return std::move(magicOutput[outputScanline++]);
  } else {
    return nullptr;
  }
}

//
// Bicubic kernel
//

namespace {
static inline float clamp(const float f, const float min, const float max) {
  return f < min ? min : (f > max ? max : f);
}

static inline float bicubicCompute(
    const float deltaX,
    const float p0,
    const float p1,
    const float p2,
    const float p3) {
  // compute cubic hermite interpolation using the horner method
  const float a = p1;
  const float b = p2 - p0;
  const float c = 2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3;
  const float d = 3.0f * (p1 - p2) + p3 - p0;
  return a + 0.5f * deltaX * (b + deltaX * (c + deltaX * d));
}

static std::vector<std::uint8_t> bicubicComputeAndClamp(
    const std::uint8_t numberOfComponents,
    const float deltaX,
    const std::uint8_t* pix0,
    const std::uint8_t* pix1,
    const std::uint8_t* pix2,
    const std::uint8_t* pix3) {
  auto result = std::vector<std::uint8_t>(numberOfComponents);

  for (std::uint8_t componentOffset = 0; componentOffset < numberOfComponents;
       componentOffset++) {
    const auto resultValue = bicubicCompute(
        deltaX,
        static_cast<float>(pix0[componentOffset]),
        static_cast<float>(pix1[componentOffset]),
        static_cast<float>(pix2[componentOffset]),
        static_cast<float>(pix3[componentOffset]));
    result[componentOffset] = clamp(resultValue, 0, 255);
  }
  return result;
}

} // namespace

class BicubicScalingBlockImpl : public ScalingBlockImpl {
 public:
  BicubicScalingBlockImpl(
      const image::pixel::Specification& pixelSpecification,
      const image::Size& inputSize,
      const image::Size& outputSize);
  ~BicubicScalingBlockImpl() override = default;
  std::unique_ptr<image::Scanline> produce() override;
};

BicubicScalingBlockImpl::BicubicScalingBlockImpl(
    const image::pixel::Specification& pixelSpecification,
    const image::Size& inputSize,
    const image::Size& outputSize)
    : ScalingBlockImpl(pixelSpecification, inputSize, outputSize) {}

std::unique_ptr<image::Scanline> BicubicScalingBlockImpl::produce() {
  if (outputScanline == outputSize.height) {
    return nullptr;
  }

  auto result =
      std::make_unique<image::Scanline>(_pixelSpecification, outputSize.width);

  const float middleY = 0.5f * invScalingY *
      static_cast<float>(outputScanline + outputScanline + 1);

  // shift from pixel center to logical index
  const float logicalMiddleY = clamp(middleY - 0.5f, 0.0f, inputSize.height);

  const int y1 = static_cast<int>(floor(logicalMiddleY));
  const int y2 = y1 < inputSize.height - 1 ? y1 + 1 : y1;
  SPECTRUM_ENFORCE_IF_NOT(y1 >= 0 && y1 <= y2 && y2 < inputSize.height);

  const float deltaY = (y1 == y2) ? 0 : (logicalMiddleY - y1) / (y2 - y1);
  SPECTRUM_ENFORCE_IF_NOT(deltaY >= 0.0f && deltaY <= 1.0f);

  const int y0 = y1 == 0 ? 0 : y1 - 1;
  const int y3 = y2 < inputSize.height - 1 ? y2 + 1 : y2;
  SPECTRUM_ENFORCE_IF_NOT(y0 >= 0 && y0 <= y3 && y3 < inputSize.height);

  if (static_cast<size_t>(y3) >= input.size()) {
    return nullptr;
  }

  for (int xOffset = 0; xOffset < outputSize.width; xOffset++) {
    const float middleX =
        0.5f * invScalingX * static_cast<float>(xOffset + xOffset + 1);

    // shift from pixel center to logical index
    const float logicalMiddleX = clamp(middleX - 0.5f, 0.0f, inputSize.width);

    const int x1 = static_cast<int>(floor(logicalMiddleX));
    const int x2 = x1 < inputSize.width - 1 ? x1 + 1 : x1;
    SPECTRUM_ENFORCE_IF_NOT(x1 >= 0 && x1 <= x2 && x2 < inputSize.width);

    const float deltaX = (x1 == x2) ? 0 : (logicalMiddleX - x1) / (x2 - x1);
    SPECTRUM_ENFORCE_IF_NOT(deltaX >= 0.0f && deltaX <= 1.0f);

    const int x0 = x1 == 0 ? 0 : x1 - 1;
    const int x3 = x2 < inputSize.width - 1 ? x2 + 1 : x2;
    SPECTRUM_ENFORCE_IF_NOT(x0 >= 0 && x0 <= x3 && x3 < inputSize.width);

    const auto numberOfComponents = _pixelSpecification.numberOfComponents();
    const auto bicubicSum0 = bicubicComputeAndClamp(
        numberOfComponents,
        deltaX,
        (*input[y0]).dataAtPixel(x0),
        (*input[y0]).dataAtPixel(x1),
        (*input[y0]).dataAtPixel(x2),
        (*input[y0]).dataAtPixel(x3));

    const auto bicubicSum1 = bicubicComputeAndClamp(
        numberOfComponents,
        deltaX,
        (*input[y1]).dataAtPixel(x0),
        (*input[y1]).dataAtPixel(x1),
        (*input[y1]).dataAtPixel(x2),
        (*input[y1]).dataAtPixel(x3));

    const auto bicubicSum2 = bicubicComputeAndClamp(
        numberOfComponents,
        deltaX,
        (*input[y2]).dataAtPixel(x0),
        (*input[y2]).dataAtPixel(x1),
        (*input[y2]).dataAtPixel(x2),
        (*input[y2]).dataAtPixel(x3));

    const auto bicubicSum3 = bicubicComputeAndClamp(
        numberOfComponents,
        deltaX,
        (*input[y3]).dataAtPixel(x0),
        (*input[y3]).dataAtPixel(x1),
        (*input[y3]).dataAtPixel(x2),
        (*input[y3]).dataAtPixel(x3));

    const auto pxl = bicubicComputeAndClamp(
        numberOfComponents,
        deltaY,
        bicubicSum0.data(),
        bicubicSum1.data(),
        bicubicSum2.data(),
        bicubicSum3.data());

    auto dst = result->dataAtPixel(xOffset);
    for (std::uint8_t componentOffset = 0; componentOffset < numberOfComponents;
         componentOffset++) {
      *(dst + componentOffset) = pxl[componentOffset];
    }
  }

  // free scanlines that will not be touched again
  for (int i = nextLineToRelease; i < y0; i++) {
    SPECTRUM_ENFORCE_IF(input[i] == nullptr);
    input[i].release();
  }
  nextLineToRelease = y0;

  ++outputScanline;
  return result;
}

//
// No-operation
//

class NoOpScalingBlockImpl : public ScalingBlockImpl {
 private:
 public:
  NoOpScalingBlockImpl(
      const image::pixel::Specification& pixelSpecification,
      const image::Size& inputSize,
      const image::Size& outputSize);
  ~NoOpScalingBlockImpl() override = default;
  std::unique_ptr<image::Scanline> produce() override;
};

NoOpScalingBlockImpl::NoOpScalingBlockImpl(
    const image::pixel::Specification& pixelSpecification,
    const image::Size& inputSize,
    const image::Size& outputSize)
    : ScalingBlockImpl(pixelSpecification, inputSize, outputSize) {
  SPECTRUM_ENFORCE_IF_NOT(inputSize == outputSize);
}

std::unique_ptr<image::Scanline> NoOpScalingBlockImpl::produce() {
  if (outputScanline == outputSize.height) {
    return nullptr;
  }
  return std::move(input[outputScanline++]);
}

//
// ScalingScanlineProcessingBlock
//

ScalingScanlineProcessingBlock::ScalingScanlineProcessingBlock(
    const image::pixel::Specification& pixelSpecification,
    const image::Size& inputSize,
    const image::Size& outputSize,
    const Configuration::General::SamplingMethod samplingMethod)
    : _pixelSpecification(pixelSpecification) {
  if (inputSize == outputSize) {
    delegate = std::make_unique<NoOpScalingBlockImpl>(
        pixelSpecification, inputSize, outputSize);
  } else {
    switch (samplingMethod) {
      case Configuration::General::SamplingMethod::MagicKernel:
        delegate = std::make_unique<MagicKernelScalingBlockImpl>(
            pixelSpecification, inputSize, outputSize);
        break;
      case Configuration::General::SamplingMethod::Bicubic:
        delegate = std::make_unique<BicubicScalingBlockImpl>(
            pixelSpecification, inputSize, outputSize);
        break;
      default:
        SPECTRUM_UNREACHABLE_CONFIGURATION_SAMPLING_METHOD(samplingMethod);
    }
  }
};

// required as `ScalingBlockImpl` is forward declared
ScalingScanlineProcessingBlock::~ScalingScanlineProcessingBlock(){};

void ScalingScanlineProcessingBlock::consume(
    std::unique_ptr<image::Scanline> scanline) {
  SPECTRUM_ENFORCE_IF_NOT(scanline->specification() == _pixelSpecification);
  delegate->consume(std::move(scanline));
}

std::unique_ptr<image::Scanline> ScalingScanlineProcessingBlock::produce() {
  return delegate->produce();
}

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
