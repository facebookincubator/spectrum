// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>

#include <folly/Optional.h>
#include <folly/Range.h>

#include <array>
#include <vector>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace isobmff {

namespace error {
extern const folly::StringPiece IsoBmffEarlyStreamEnd;
extern const folly::StringPiece IsoBmffBoxTooLarge;
extern const folly::StringPiece IsoBmffFtypBoxTooSmall;
extern const folly::StringPiece IsoBmffFtypBoxExpectedButNotFound;
} // namespace error

using BoxType = std::array<std::uint8_t, 4>;
using UserType = std::array<std::uint8_t, 16>;
using Brand = std::array<std::uint8_t, 4>;

/** ISO/IEC 14496-12:201 - 4.2 */
struct BoxHeader {
  folly::Optional<std::uint64_t> size = {0};
  BoxType boxType;
  UserType userType;

  static constexpr auto BoxTypeUuid = folly::StringPiece{"uuid"};
};

/** ISO/IEC 14496-12:201 - 4.3.2 */
struct FtypBox {
  BoxHeader boxHeader;
  Brand majorBrand;
  std::array<std::uint8_t, 4> minorVersion;
  std::vector<Brand> compatibleBrands;
};

/**
 * An incomplete ISO-BMFF parser that can just the FTYP in a MiAF compatible
 * file (i.e. the FTYP is right at the beginning of the file)
 */
class Parser {
 public:
  /**
   * Creates a new parser for the given image source. The `maxBoxSize` can be
   * set to make the parser robust against exceedingly long input boxes
   */
  Parser(
      io::IImageSource& _source,
      const std::uint64_t maxBoxSize =
          std::numeric_limits<std::uint64_t>::max());

  /**
   * Reads the next box header. The header is expected to start at the current
   * position in IImageSource.
   */
  BoxHeader parseBoxHeader();

  /**
   * Reads the next FTYP box. The box is expected to start at the current
   * position in the IImageSource.
   *
   * Throws if there is not FTYP box at the given location, or if the stream
   * doesn't conform to the specification, or if the box size exeeds maxBoxSize
   * or any other error occurs.
   */
  FtypBox parseFtypBox();

 private:
  io::IImageSource& _source;
  std::uint64_t _maxBoxSize;
};

} // namespace isobmff
} // namespace codecs
} // namespace spectrum
} // namespace facebook
