// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/FixedString.h>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * Represents an image format. The sub-class `EncodedFormat` indicates that the
 * format is encoded.
 *
 * The `formats` namespace below contains pre-defined formats.
 */
class Format {
 public:
  using Identifier = folly::FixedString<10>;

  constexpr Format(const Identifier& identifier, const bool encoded = false)
      : _identifier(identifier), _encoded(encoded) {}

  /**
   * Unique identifier of this format. E.g. "jpeg"
   */
  const Identifier& identifier() const;

  /**
   * Returns true if the image format is encoded
   */
  bool encoded() const;

  std::string string() const;
  bool operator==(const Format& rhs) const;
  bool operator!=(const Format& rhs) const;

 protected:
  Identifier _identifier;
  bool _encoded;
};

/**
 * Represents a compressed image format.

 * The `formats` namespace below contains pre-defined formats.
 */
class EncodedFormat : public Format {
 public:
  constexpr EncodedFormat(const Identifier& identifier)
      : Format(identifier, true) {}
};

static_assert(
    sizeof(Format) == sizeof(EncodedFormat),
    "We rely on object slicing, EncodedFormat should not add anything");

/**
 * Predefined image formats.
 */
namespace formats {

/** An uncompressed image of continious pixel data */
static constexpr Format Bitmap{"bitmap"};

/** A JPEG encoded image, see: https://en.wikipedia.org/wiki/JPEG */
static constexpr EncodedFormat Jpeg{"jpeg"};

/**
 * A PNG encoded image, see
 * https://en.wikipedia.org/wiki/Portable_Network_Graphics
 */
static constexpr EncodedFormat Png{"png"};

/** A WebP encoded image, see https://en.wikipedia.org/wiki/WebP */
static constexpr EncodedFormat Webp{"webp"};

/** A GIF encoded image, see https://en.wikipedia.org/wiki/GIF */
static constexpr EncodedFormat Gif{"gif"};

/**
 * A HEIF encoded image, see
 * https://en.wikipedia.org/wiki/High_Efficiency_Image_File_Format
 */
static constexpr EncodedFormat Heif{"heif"};

} // namespace formats
} // namespace image
} // namespace spectrum
} // namespace facebook
