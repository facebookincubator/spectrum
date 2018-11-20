// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ScanlineConversion.h"

#include <spectrum/image/Color.h>
#include <spectrum/image/Pixel.h>
#include <spectrum/image/Scanline.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

namespace error {
const folly::StringPiece CannotConvertScanlineBetweenSpecifications{
    "cannot_convert_scanline_between_specifications"};
const folly::StringPiece UnsupportedPremultipliedAlpha{
    "scanline_conversion_unsupported_premultiplied_alpha"};
}

namespace {

constexpr std::uint8_t fullAlpha = 255;

struct Pixel_1 {
  std::uint8_t x0;
} __attribute__((packed));

struct Pixel_3 {
  std::uint8_t x0;
  std::uint8_t x1;
  std::uint8_t x2;
} __attribute__((packed));

struct Pixel_4 {
  std::uint8_t x0;
  std::uint8_t x1;
  std::uint8_t x2;
  std::uint8_t x3;
} __attribute__((packed));

} // namespace

namespace convert {

inline void grayToRgb(const Pixel_1& in, Pixel_3& out, const image::Color&) {
  out = {in.x0, in.x0, in.x0};
}

inline void grayToRgba(const Pixel_1& in, Pixel_4& out, const image::Color&) {
  out = {in.x0, in.x0, in.x0, fullAlpha};
}

inline void grayToArgb(const Pixel_1& in, Pixel_4& out, const image::Color&) {
  out = {fullAlpha, in.x0, in.x0, in.x0};
}

inline void rgbToGray(const Pixel_3& in, Pixel_1& out, const image::Color&) {
  const auto average = (in.x0 + in.x1 + in.x2) / 3;
  out = {static_cast<std::uint8_t>(average)};
}

inline void rgbToRgba(const Pixel_3& in, Pixel_4& out, const image::Color&) {
  out = {in.x0, in.x1, in.x2, fullAlpha};
}

inline void rgbToArgb(const Pixel_3& in, Pixel_4& out, const image::Color&) {
  out = {fullAlpha, in.x0, in.x1, in.x2};
}

inline void argbToRgba(const Pixel_4& in, Pixel_4& out, const image::Color&) {
  out = {in.x1, in.x2, in.x3, in.x0};
}

inline void rgbaToArgb(const Pixel_4& in, Pixel_4& out, const image::Color&) {
  out = {in.x3, in.x0, in.x1, in.x2};
}

} // namespace convert

namespace convertwithbackground {

Pixel_3 blendPixelWithBackgroundToRgb(
    const std::uint8_t red,
    const std::uint8_t green,
    const std::uint8_t blue,
    const std::uint8_t pixelAlpha,
    const image::Color& background) {
  if (pixelAlpha == fullAlpha) {
    return Pixel_3{red, green, blue};
  } else if (pixelAlpha == 0) {
    return Pixel_3{background.red, background.green, background.blue};
  } else {
    const float alpha = pixelAlpha / static_cast<float>(fullAlpha);
    const float inverseAlpha = 1.0f - alpha;
    return Pixel_3{
        std::uint8_t(red * alpha + background.red * inverseAlpha),
        std::uint8_t(green * alpha + background.green * inverseAlpha),
        std::uint8_t(blue * alpha + background.blue * inverseAlpha)};
  }
}

inline void
argbToRgb(const Pixel_4& in, Pixel_3& out, const image::Color& background) {
  out = blendPixelWithBackgroundToRgb(in.x1, in.x2, in.x3, in.x0, background);
}

inline void
rgbaToRgb(const Pixel_4& in, Pixel_3& out, const image::Color& background) {
  out = blendPixelWithBackgroundToRgb(in.x0, in.x1, in.x2, in.x3, background);
}

inline void
argbToGray(const Pixel_4& in, Pixel_1& out, const image::Color& background) {
  Pixel_3 rgb;
  argbToRgb(in, rgb, background);
  convert::rgbToGray(rgb, out, background);
}

inline void
rgbaToGray(const Pixel_4& in, Pixel_1& out, const image::Color& background) {
  Pixel_3 rgb;
  rgbaToRgb(in, rgb, background);
  convert::rgbToGray(rgb, out, background);
}

} // namespace convertwithbackground

namespace {
inline std::uint8_t _extractAlpha(
    const std::uint8_t* const pixel,
    const bool hasAlpha,
    const PixelIndex alphaIndex) {
  return hasAlpha ? pixel[alphaIndex] : fullAlpha;
}

inline std::uint8_t _extractComponent(
    const std::uint8_t* const pixel,
    const PixelIndex componentIndex,
    const bool inputHasAlpha,
    const bool outputHasAlpha,
    const PixelIndex alphaIndex,
    const std::uint8_t backgroundColorComponent) {
  auto component = pixel[componentIndex];

  if (inputHasAlpha && !outputHasAlpha) {
    // We need to blend the component with the background
    const auto alphaValue = pixel[alphaIndex];

    if (alphaValue == 0) {
      component = backgroundColorComponent;
    } else if (alphaValue < fullAlpha) {
      const auto alphaFloat = alphaValue / static_cast<float>(fullAlpha);
      component = (alphaFloat * component) +
          ((1.0f - alphaFloat) * backgroundColorComponent);
    }
  }

  // TODO T33763929: De-premultiply or pre-multiply components depending on
  // source / target alphaInfo
  return component;
}

template <typename InputIndices, typename OutputIndices>
inline void _convertDynamicPixel(
    const InputIndices& inputIndices,
    const std::uint8_t* const inputPixel,
    const OutputIndices& outputIndices,
    std::uint8_t* const outputPixel,
    const image::Color& backgroundColor);

template <>
inline void _convertDynamicPixel(
    const indices::RGB& inputIndices,
    const std::uint8_t* const inputPixel,
    const indices::RGB& outputIndices,
    std::uint8_t* const outputPixel,
    const image::Color& backgroundColor) {
  outputPixel[outputIndices.red] = _extractComponent(
      inputPixel,
      inputIndices.red,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.red);
  outputPixel[outputIndices.green] = _extractComponent(
      inputPixel,
      inputIndices.green,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.green);
  outputPixel[outputIndices.blue] = _extractComponent(
      inputPixel,
      inputIndices.blue,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.blue);
}

template <>
inline void _convertDynamicPixel(
    const indices::RGB& inputIndices,
    const std::uint8_t* const inputPixel,
    const indices::Gray& outputIndices,
    std::uint8_t* const outputPixel,
    const image::Color& backgroundColor) {
  const auto red = _extractComponent(
      inputPixel,
      inputIndices.red,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.red);
  const auto green = _extractComponent(
      inputPixel,
      inputIndices.green,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.green);
  const auto blue = _extractComponent(
      inputPixel,
      inputIndices.blue,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.blue);

  outputPixel[outputIndices.gray] = (red + green + blue) / 3;
}

template <>
inline void _convertDynamicPixel(
    const indices::Gray& inputIndices,
    const std::uint8_t* const inputPixel,
    const indices::RGB& outputIndices,
    std::uint8_t* const outputPixel,
    const image::Color& backgroundColor) {
  const auto gray = _extractComponent(
      inputPixel,
      inputIndices.gray,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.gray());

  outputPixel[outputIndices.red] = gray;
  outputPixel[outputIndices.green] = gray;
  outputPixel[outputIndices.blue] = gray;
}

template <>
inline void _convertDynamicPixel(
    const indices::Gray& inputIndices,
    const std::uint8_t* const inputPixel,
    const indices::Gray& outputIndices,
    std::uint8_t* const outputPixel,
    const image::Color& backgroundColor) {
  outputPixel[outputIndices.gray] = _extractComponent(
      inputPixel,
      inputIndices.gray,
      inputIndices.hasAlpha,
      outputIndices.hasAlpha,
      inputIndices.alpha,
      backgroundColor.gray());
}

} // namespace

namespace indices {

RGB::RGB(const image::pixel::Specification& specification) {
  // We start with the default values: RGBA

  SPECTRUM_ENFORCE_IF_NOT(
      specification.colorModel == image::pixel::colormodels::RGB);

  if (specification.componentsOrder ==
      image::pixel::ComponentsOrder::Reversed) {
    std::swap(red, blue);
  }

  if (specification.isAlphaLeadingComponent()) {
    // The alpha is first so push everything to the end
    alpha = 0;
    red++;
    green++;
    blue++;
  }

  // Remove alpha component if the value shoudn't be interpreted (skipFirst,
  // skipLast, none).
  hasAlpha =
      image::pixel::alphaInfoShouldInterpretAlphaValue(specification.alphaInfo);
}

Gray::Gray(const image::pixel::Specification& specification) {
  // We start with the default values: GA

  SPECTRUM_ENFORCE_IF_NOT(
      specification.colorModel == image::pixel::colormodels::Gray);

  if (specification.isAlphaLeadingComponent()) {
    std::swap(alpha, gray);
  }

  // Remove alpha component if the value shoudn't be interpreted (skipFirst,
  // skipLast, none).
  hasAlpha =
      image::pixel::alphaInfoShouldInterpretAlphaValue(specification.alphaInfo);
}
} // namespace indices

//
// ScanlineConverter
//

ScanlineConverter::ScanlineConverter(
    const image::pixel::Specification& inputSpecification,
    const image::pixel::Specification& outputSpecification,
    const image::Color& backgroundColor)
    : _inputSpecification(inputSpecification),
      _outputSpecification(outputSpecification),
      _backgroundColor(backgroundColor) {}

//
// DynamicScanlineConverter
//

template <typename InputIndices, typename OutputIndices>
DynamicScanlineConverter<InputIndices, OutputIndices>::DynamicScanlineConverter(
    const image::pixel::Specification& inputSpecification,
    const image::pixel::Specification& outputSpecification,
    const image::Color& backgroundColor)
    : ScanlineConverter(
          inputSpecification,
          outputSpecification,
          backgroundColor),
      _inputIndices(inputSpecification),
      _outputIndices(outputSpecification) {
  SPECTRUM_ERROR_IF(
      image::pixel::alphaInfoIsPremultiplied(inputSpecification.alphaInfo),
      error::UnsupportedPremultipliedAlpha);
  SPECTRUM_ERROR_IF(
      image::pixel::alphaInfoIsPremultiplied(outputSpecification.alphaInfo),
      error::UnsupportedPremultipliedAlpha);
}

template <typename InputIndices, typename OutputIndices>
std::unique_ptr<image::Scanline>
DynamicScanlineConverter<InputIndices, OutputIndices>::convertScanline(
    std::unique_ptr<image::Scanline> input) const {
  SPECTRUM_ENFORCE_IF_NOT(input->specification() == this->_inputSpecification);

  auto output = std::make_unique<image::Scanline>(
      this->_outputSpecification, input->width());

  for (std::size_t i = 0; i < input->width(); ++i) {
    const auto inputPixel = input->dataAtPixel(i);
    const auto outputPixel = output->dataAtPixel(i);

    _convertDynamicPixel(
        _inputIndices,
        inputPixel,
        _outputIndices,
        outputPixel,
        this->_backgroundColor);

    if (_outputIndices.hasAlpha) {
      outputPixel[_outputIndices.alpha] = _extractAlpha(
          inputPixel, _inputIndices.hasAlpha, _inputIndices.alpha);
    }
  }

  return output;
}

//
// DefaultScanlineConverter
//

template <
    typename PI,
    typename PO,
    void _pixelConversionFunction(const PI&, PO&, const image::Color&)>
DefaultScanlineConverter<PI, PO, _pixelConversionFunction>::
    DefaultScanlineConverter(
        const image::pixel::Specification& inputSpecification,
        const image::pixel::Specification& outputSpecification,
        const image::Color& backgroundColor)
    : ScanlineConverter(
          inputSpecification,
          outputSpecification,
          backgroundColor) {}

template <
    typename PI,
    typename PO,
    void _pixelConversionFunction(const PI&, PO&, const image::Color&)>
std::unique_ptr<image::Scanline>
DefaultScanlineConverter<PI, PO, _pixelConversionFunction>::convertScanline(
    std::unique_ptr<image::Scanline> input) const {
  SPECTRUM_ENFORCE_IF_NOT(input->specification() == this->_inputSpecification);

  auto output = std::make_unique<image::Scanline>(
      this->_outputSpecification, input->width());

  for (std::size_t i = 0; i < input->width(); ++i) {
    _pixelConversionFunction(
        reinterpret_cast<const PI*>(input->data())[i],
        reinterpret_cast<PO*>(output->data())[i],
        this->_backgroundColor);
  }

  return output;
}

//
// NoOpScanlineConverter
//

NoOpScanlineConverter::NoOpScanlineConverter(
    const image::pixel::Specification& inputSpecification,
    const image::pixel::Specification& outputSpecification,
    const image::Color& backgroundColor)
    : ScanlineConverter(
          inputSpecification,
          outputSpecification,
          backgroundColor) {}

std::unique_ptr<image::Scanline> NoOpScanlineConverter::convertScanline(
    std::unique_ptr<image::Scanline> input) const {
  return input;
}

//
// ScanlineConverter Factory
//

std::unique_ptr<ScanlineConverter> makeScanlineConverter(
    const image::pixel::Specification& inputSpecification,
    const image::pixel::Specification& outputSpecification,
    const image::Color& backgroundColor) {
  if (inputSpecification == outputSpecification) {
    return std::make_unique<NoOpScanlineConverter>(
        inputSpecification, outputSpecification, backgroundColor);
  } else if (
      inputSpecification == image::pixel::specifications::Gray &&
      outputSpecification == image::pixel::specifications::RGB) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_1, Pixel_3, convert::grayToRgb>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::Gray &&
      outputSpecification == image::pixel::specifications::ARGB) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_1, Pixel_4, convert::grayToArgb>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::Gray &&
      outputSpecification == image::pixel::specifications::RGBA) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_1, Pixel_4, convert::grayToRgba>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::RGB &&
      outputSpecification == image::pixel::specifications::Gray) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_3, Pixel_1, convert::rgbToGray>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::RGB &&
      outputSpecification == image::pixel::specifications::ARGB) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_3, Pixel_4, convert::rgbToArgb>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::RGB &&
      outputSpecification == image::pixel::specifications::RGBA) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_3, Pixel_4, convert::rgbToRgba>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::ARGB &&
      outputSpecification == image::pixel::specifications::RGBA) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_4, Pixel_4, convert::argbToRgba>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::RGBA &&
      outputSpecification == image::pixel::specifications::ARGB) {
    return std::make_unique<
        DefaultScanlineConverter<Pixel_4, Pixel_4, convert::rgbaToArgb>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::ARGB &&
      outputSpecification == image::pixel::specifications::Gray) {
    return std::make_unique<DefaultScanlineConverter<
        Pixel_4,
        Pixel_1,
        convertwithbackground::argbToGray>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::RGBA &&
      outputSpecification == image::pixel::specifications::Gray) {
    return std::make_unique<DefaultScanlineConverter<
        Pixel_4,
        Pixel_1,
        convertwithbackground::rgbaToGray>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::ARGB &&
      outputSpecification == image::pixel::specifications::RGB) {
    return std::make_unique<DefaultScanlineConverter<
        Pixel_4,
        Pixel_3,
        convertwithbackground::argbToRgb>>(
        inputSpecification, outputSpecification, backgroundColor);

  } else if (
      inputSpecification == image::pixel::specifications::RGBA &&
      outputSpecification == image::pixel::specifications::RGB) {
    return std::make_unique<DefaultScanlineConverter<
        Pixel_4,
        Pixel_3,
        convertwithbackground::rgbaToRgb>>(
        inputSpecification, outputSpecification, backgroundColor);
  } else {
    // Fallback to dynamic conversion. It is currently a tad slower than the
    // the default converter (about 80ms for a 4k image on iPhone 7).
    if (inputSpecification.colorModel == image::pixel::colormodels::RGB) {
      if (outputSpecification.colorModel == image::pixel::colormodels::RGB) {
        return std::make_unique<
            DynamicScanlineConverter<indices::RGB, indices::RGB>>(
            inputSpecification, outputSpecification, backgroundColor);
      } else if (
          outputSpecification.colorModel == image::pixel::colormodels::Gray) {
        return std::make_unique<
            DynamicScanlineConverter<indices::RGB, indices::Gray>>(
            inputSpecification, outputSpecification, backgroundColor);
      }
    } else if (
        inputSpecification.colorModel == image::pixel::colormodels::Gray) {
      if (outputSpecification.colorModel == image::pixel::colormodels::RGB) {
        return std::make_unique<
            DynamicScanlineConverter<indices::Gray, indices::RGB>>(
            inputSpecification, outputSpecification, backgroundColor);
      } else if (
          outputSpecification.colorModel == image::pixel::colormodels::Gray) {
        return std::make_unique<
            DynamicScanlineConverter<indices::Gray, indices::Gray>>(
            inputSpecification, outputSpecification, backgroundColor);
      }
    }

    SPECTRUM_ERROR_FORMAT(
        error::CannotConvertScanlineBetweenSpecifications,
        "in: %s, out: %s",
        inputSpecification.string().c_str(),
        outputSpecification.string().c_str());
  }
}

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
