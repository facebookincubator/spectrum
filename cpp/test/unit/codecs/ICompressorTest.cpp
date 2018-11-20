// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/Repository.h>
#include <spectrum/image/metadata/Entries.h>
#include <spectrum/testutils/TestUtils.h>

#include <exception>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace test {

TEST(
    codecs_ICompressor,
    whenPassingNoEncodeRequirement_thenEnforceLosslessSucceeds) {
  ASSERT_NO_THROW(ICompressor::enforceLossless(folly::none));
}

TEST(
    codecs_ICompressor,
    whenPassingLosslessRequirement_thenEnforceLosslessSucceeds) {
  const auto losslessRequirement = requirements::Encode{
      .format = image::formats::Png,
      .quality = requirements::Encode::QualityDefault,
      .mode = requirements::Encode::Mode::Lossless,
  };

  ASSERT_NO_THROW(ICompressor::enforceLossless(losslessRequirement));
}

TEST(codecs_ICompressor, whenPassingLossyRequirement_thenEnforceLosslessFails) {
  const auto lossyRequirement = requirements::Encode{
      .format = image::formats::Jpeg,
      .quality = requirements::Encode::QualityDefault,
      .mode = requirements::Encode::Mode::Lossy,
  };

  ASSERT_SPECTRUM_THROW(
      ICompressor::enforceLossless(lossyRequirement),
      error::CompressorRequiresLossless);
}

TEST(
    codecs_ICompressor,
    whenPassingNoEncodeRequirement_thenEnforceLossySucceeds) {
  ASSERT_NO_THROW(ICompressor::enforceLossy(folly::none));
}

TEST(codecs_ICompressor, whenPassingLossyRequirement_thenEnforceLossySucceeds) {
  const auto lossyRequirement = requirements::Encode{
      .format = image::formats::Jpeg,
      .quality = requirements::Encode::QualityDefault,
      .mode = requirements::Encode::Mode::Lossy,
  };

  ASSERT_NO_THROW(ICompressor::enforceLossy(lossyRequirement));
}

TEST(codecs_ICompressor, whenPassingLosslessRequirement_thenEnforceLossyFails) {
  const auto losslessRequirement = requirements::Encode{
      .format = image::formats::Png,
      .quality = requirements::Encode::QualityDefault,
      .mode = requirements::Encode::Mode::Lossless,
  };

  ASSERT_SPECTRUM_THROW(
      ICompressor::enforceLossy(losslessRequirement),
      error::CompressorRequiresLossy);
}

TEST(
    codecs_ICompressor,
    whenPassingSomeMetadata_thenEnforceCannotEncodeMetadataFails) {
  auto someMetadata = image::Metadata{};

  someMetadata.entries().setOrientation(image::Orientation::Bottom);

  ASSERT_SPECTRUM_THROW(
      ICompressor::enforceCannotEncodeMetadata(someMetadata),
      error::CompressorCannotEncodeMetadata);
}

TEST(
    codecs_ICompressor,
    whenPassingSomePixelSpecification_thenEnforcePixelSpecificationSetSucceeds) {
  const auto somePixelSpecification = image::pixel::specifications::RGB;

  ASSERT_NO_THROW(
      ICompressor::enforcePixelSpecificationSet(somePixelSpecification));
}

TEST(
    codecs_ICompressor,
    whenPassingNoPixelSpecification_thenEnforcePixelSpecificationSetFails) {
  folly::Optional<image::pixel::Specification> noPixelSpecification =
      folly::none;

  ASSERT_SPECTRUM_THROW(
      ICompressor::enforcePixelSpecificationSet(noPixelSpecification),
      error::CompressorRequiresPixelSpecification);
}

TEST(
    codecs_ICompressor,
    whenPassingInBoundsImageDimensions_thenEnforceImageDimensionsPasses) {
  const auto size = image::Size{
      .width = 1,
      .height = 1,
  };
  const auto maxDimension = std::uint32_t{1};
  ASSERT_NO_THROW(
      ICompressor::enforceSizeBelowMaximumSideDimension(size, maxDimension));
}

TEST(
    codecs_ICompressor,
    whenPassingOverboundsImageDimensions_thenEnforceImageDimensionsFails) {
  const auto size = image::Size{
      .width = 2,
      .height = 2,
  };
  const auto maxDimension = std::uint32_t{1};
  ASSERT_SPECTRUM_THROW(
      ICompressor::enforceSizeBelowMaximumSideDimension(size, maxDimension),
      error::CompressorInputSizeTooLarge);
}

} // namespace test
} // namespace codecs
} // namespace spectrum
} // namespace facebook
