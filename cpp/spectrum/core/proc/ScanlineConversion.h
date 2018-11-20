// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Color.h>
#include <spectrum/image/Pixel.h>
#include <spectrum/image/Scanline.h>

#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

namespace error {
extern const folly::StringPiece CannotConvertScanlineBetweenSpecifications;
extern const folly::StringPiece UnsupportedPremultipliedAlpha;
}

using PixelIndex = std::uint8_t;

namespace indices {

struct RGB {
  PixelIndex red{0};
  PixelIndex green{1};
  PixelIndex blue{2};
  PixelIndex alpha{3};

  bool hasAlpha{true};

  RGB(const image::pixel::Specification& specification);
};

struct Gray {
  PixelIndex gray{0};
  PixelIndex alpha{1};

  bool hasAlpha{true};

  Gray(const image::pixel::Specification& specification);
};
} // namespace indices

class ScanlineConverter {
 public:
  explicit ScanlineConverter(
      const image::pixel::Specification& inputSpecification,
      const image::pixel::Specification& outputSpecification,
      const image::Color& backgroundColor);

  virtual ~ScanlineConverter() = default;

  virtual std::unique_ptr<image::Scanline> convertScanline(
      std::unique_ptr<image::Scanline> input) const = 0;

 protected:
  image::pixel::Specification _inputSpecification;
  image::pixel::Specification _outputSpecification;
  image::Color _backgroundColor;
};

template <
    typename PI,
    typename PO,
    void _pixelConversionFunction(const PI&, PO&, const image::Color&)>
class DefaultScanlineConverter : public ScanlineConverter {
 public:
  DefaultScanlineConverter(
      const image::pixel::Specification& inputSpecification,
      const image::pixel::Specification& outputSpecification,
      const image::Color& backgroundColor);

  ~DefaultScanlineConverter() override = default;

  std::unique_ptr<image::Scanline> convertScanline(
      std::unique_ptr<image::Scanline> input) const override;
};

template <typename InputIndices, typename OutputIndices>
class DynamicScanlineConverter : public ScanlineConverter {
 public:
  DynamicScanlineConverter(
      const image::pixel::Specification& inputSpecification,
      const image::pixel::Specification& outputSpecification,
      const image::Color& backgroundColor);

  ~DynamicScanlineConverter() override = default;

  std::unique_ptr<image::Scanline> convertScanline(
      std::unique_ptr<image::Scanline> input) const override;

 private:
  InputIndices _inputIndices;
  OutputIndices _outputIndices;
};

class NoOpScanlineConverter : public ScanlineConverter {
 public:
  NoOpScanlineConverter(
      const image::pixel::Specification& inputSpecification,
      const image::pixel::Specification& outputSpecification,
      const image::Color& backgroundColor);

  ~NoOpScanlineConverter() override = default;

  std::unique_ptr<image::Scanline> convertScanline(
      std::unique_ptr<image::Scanline> input) const override;
};

std::unique_ptr<ScanlineConverter> makeScanlineConverter(
    const image::pixel::Specification& inputSpecification,
    const image::pixel::Specification& outputSpecification,
    const image::Color& backgroundColor);

} // namespace proc
} // namespace core

} // namespace spectrum
} // namespace facebook
