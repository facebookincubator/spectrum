// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/EncodedImageFormatDetectorHandlers.h>
#include <spectrum/Spectrum.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <folly/Range.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace test {
namespace headers {
constexpr auto Jpeg = folly::StringPiece("\xFF\xD8\xFF");
constexpr auto Png = folly::StringPiece("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A");
constexpr auto Gif87a = folly::StringPiece("\x47\x49\x46\x38\x37\x61");
constexpr auto Gif89a = folly::StringPiece("\x47\x49\x46\x38\x39\x61");
constexpr auto WebpVp8 = folly::StringPiece(
    "\x52\x49\x46\x46\x0A\xAA\xBB\x0B\x57\x45\x42\x50\x56\x50\x38\x20");
constexpr auto WebpVp8L = folly::StringPiece(
    "\x52\x49\x46\x46\x0A\xAA\xBB\x0B\x57\x45\x42\x50\x56\x50\x38\x4C");
constexpr auto WebpVp8X = folly::StringPiece(
    "\x52\x49\x46\x46\x0A\xAA\xBB\x0B\x57\x45\x42\x50\x56\x50\x38\x58");
} // namespace headers

namespace {

folly::Optional<image::EncodedFormat> detectImageFormat(
    const std::string& content,
    const EncodedImageFormatDetectorHandler&
        encodedImageFormatDetectorHandler) {
  auto source = io::testutils::makeVectorImageSource(content);
  return encodedImageFormatDetectorHandler(source);
}

void assertDetectImageFormatFromHeader(
    const folly::StringPiece& header,
    const image::EncodedFormat& expectedFormat,
    const EncodedImageFormatDetectorHandler&
        encodedImageFormatDetectorHandler) {
  const auto headerString = header.str();

  ASSERT_GE(headerString.size(), 1);

  const auto detectedFormat =
      detectImageFormat(headerString, encodedImageFormatDetectorHandler);

  ASSERT_EQ(expectedFormat, detectedFormat);

  const auto shortHeaderString =
      headerString.substr(0, headerString.size() - 1);
  const auto detectedShortFormat =
      detectImageFormat(shortHeaderString, encodedImageFormatDetectorHandler);

  ASSERT_EQ(folly::none, detectedShortFormat);

  const auto longHeaderString = headerString + "trailingData";
  const auto detectedLongFormat =
      detectImageFormat(longHeaderString, encodedImageFormatDetectorHandler);

  ASSERT_EQ(expectedFormat, detectedLongFormat);
}
} // namespace

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenDetectingJpegSource_thenProperImageFormatReturned) {
  assertDetectImageFormatFromHeader(
      headers::Jpeg,
      image::formats::Jpeg,
      makeJpegImageFormatDetectorHandler());
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenDetectingPngSource_thenProperImageFormatReturned) {
  assertDetectImageFormatFromHeader(
      headers::Png, image::formats::Png, makePngImageFormatDetectorHandler());
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenDetectingGifSource_thenProperImageFormatReturned) {
  assertDetectImageFormatFromHeader(
      headers::Gif87a,
      image::formats::Gif,
      makeGif87ImageFormatDetectorHandler());
  assertDetectImageFormatFromHeader(
      headers::Gif89a,
      image::formats::Gif,
      makeGif89ImageFormatDetectorHandler());
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenDetectingWebpSource_thenProperImageFormatReturned) {
  assertDetectImageFormatFromHeader(
      headers::WebpVp8,
      image::formats::Webp,
      makeWebpVp8ImageFormatDetectorHandler());
  assertDetectImageFormatFromHeader(
      headers::WebpVp8L,
      image::formats::Webp,
      makeWebpVp8lImageFormatDetectorHandler());
  assertDetectImageFormatFromHeader(
      headers::WebpVp8X,
      image::formats::Webp,
      makeWebpVp8xImageFormatDetectorHandler());
}

TEST(codecs_EncodedImageFormatDetectorHandlers, whenEmpty_thenReturnNone) {
  for (auto handler : makeAllImageFormatDetectorHandlers()) {
    auto imageSource = io::testutils::makeVectorImageSource(std::string("", 0));
    ASSERT_EQ(folly::none, handler(imageSource));
  }
}

TEST(codecs_EncodedImageFormatDetectorHandlers, whenNotFtyp_thenReturnNone) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "BAAD",
      8));
  ASSERT_EQ(folly::none, makeIsobmffImageFormatDetectorHandler()(imageSource));
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenNoMatchingMajorBrands_thenReturnNone) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x08"
      "ftyp"
      "XXXX"
      "0000",
      8 + 8));
  ASSERT_EQ(folly::none, makeIsobmffImageFormatDetectorHandler()(imageSource));
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenMatchingMajorBrandsIsHeif_thenReturnHeif) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x08"
      "ftyp"
      "heic"
      "0000",
      8 + 8));
  ASSERT_EQ(
      image::formats::Heif,
      makeIsobmffImageFormatDetectorHandler()(imageSource));
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenMatchingCompatibleBrandsContainsHeif_thenReturnHeif) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "ftyp"
      "XXXX"
      "0000"
      "XXXX"
      "heic",
      8 + 16));
  ASSERT_EQ(
      image::formats::Heif,
      makeIsobmffImageFormatDetectorHandler()(imageSource));
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenMatchingMajorBrandsIsAvif_thenReturnAvif) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x08"
      "ftyp"
      "avif"
      "0000",
      8 + 8));
  ASSERT_EQ(
      image::formats::Avif,
      makeIsobmffImageFormatDetectorHandler()(imageSource));
}

TEST(
    codecs_EncodedImageFormatDetectorHandlers,
    whenMatchingCompatibleBrandsContainsAvif_thenReturnAvif) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "ftyp"
      "XXXX"
      "0000"
      "XXXX"
      "avif",
      8 + 16));
  ASSERT_EQ(
      image::formats::Avif,
      makeIsobmffImageFormatDetectorHandler()(imageSource));
}

} // namespace test
} // namespace codecs
} // namespace spectrum
} // namespace facebook
