// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "IsoBmffParser.h"

#include <spectrum/core/utils/Endianness.h>

#include <cstring>
#include <limits>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace isobmff {

namespace error {
const folly::StringPiece IsoBmffEarlyStreamEnd{"isobmff_early_stream_end"};
const folly::StringPiece IsoBmffBoxTooLarge{"isobmff_box_too_large"};
const folly::StringPiece IsoBmffFtypBoxTooSmall{"isobmff_ftyp_box_too_small"};
const folly::StringPiece IsoBmffFtypBoxExpectedButNotFound{
    "isobmff_ftyp_box_expected_but_not_found"};
} // namespace error

namespace boxtype {
constexpr auto uuid = folly::StringPiece{"uuid"};
constexpr auto ftyp = folly::StringPiece{"ftyp"};
} // namespace boxtype

namespace {

const bool isUuidType(const std::array<std::uint8_t, 4>& boxType) {
  return std::strncmp(
             boxtype::uuid.begin(),
             reinterpret_cast<const char*>(boxType.data()),
             boxType.size()) == 0;
}
} // namespace

Parser::Parser(io::IImageSource& source, const std::uint64_t maxBoxSize)
    : _source(source), _maxBoxSize(maxBoxSize){};

BoxHeader Parser::parseBoxHeader() {
  BoxHeader result;

  std::uint32_t size;
  SPECTRUM_ERROR_IF_NOT(
      _source.read(reinterpret_cast<char*>(&size), 4) == 4,
      error::IsoBmffEarlyStreamEnd);
  size = core::utils::convertValueToNativeByteOrder(size, false);
  SPECTRUM_ERROR_IF(size > _maxBoxSize, error::IsoBmffBoxTooLarge);

  SPECTRUM_ERROR_IF_NOT(
      _source.read(
          reinterpret_cast<char*>(result.boxType.data()),
          result.boxType.size()) == result.boxType.size(),
      error::IsoBmffEarlyStreamEnd);

  // handle special size cases: unlimited or 64-bit size
  if (size == 1) {
    std::uint64_t largeSize;
    SPECTRUM_ERROR_IF_NOT(
        _source.read(reinterpret_cast<char*>(&largeSize), 8) == 8,
        error::IsoBmffEarlyStreamEnd);
    result.size = core::utils::convertValueToNativeByteOrder(largeSize, false);
    SPECTRUM_ERROR_IF(largeSize > _maxBoxSize, error::IsoBmffBoxTooLarge);
  } else if (size == 0) {
    SPECTRUM_ERROR_IF(
        _maxBoxSize != std::numeric_limits<std::uint64_t>::max(),
        error::IsoBmffBoxTooLarge);
    result.size = folly::none;
  } else {
    result.size = size;
  }

  // read custom user ID if ftyp is `uuid`
  if (isUuidType(result.boxType)) {
    SPECTRUM_ERROR_IF_NOT(
        _source.read(
            reinterpret_cast<char*>(result.userType.data()),
            result.userType.size()) == result.userType.size(),
        error::IsoBmffEarlyStreamEnd);
  } else {
    std::memset(result.userType.data(), 0, result.userType.size());
  }

  return result;
}

FtypBox Parser::parseFtypBox() {
  FtypBox result;

  result.boxHeader = parseBoxHeader();

  // Do not allow ftyp boxes with unlimited length
  SPECTRUM_ENFORCE_IF_NOT(result.boxHeader.size.hasValue());
  SPECTRUM_ERROR_IF_NOT(
      *result.boxHeader.size >= 8, error::IsoBmffFtypBoxTooSmall);

  SPECTRUM_ERROR_IF(
      std::strncmp(
          reinterpret_cast<const char*>(result.boxHeader.boxType.data()),
          boxtype::ftyp.begin(),
          boxtype::ftyp.size()) != 0,
      error::IsoBmffFtypBoxExpectedButNotFound);

  SPECTRUM_ERROR_IF_NOT(
      _source.read(
          reinterpret_cast<char*>(result.majorBrand.data()),
          result.majorBrand.size()) == result.majorBrand.size(),
      error::IsoBmffEarlyStreamEnd);

  SPECTRUM_ERROR_IF_NOT(
      _source.read(
          reinterpret_cast<char*>(result.minorVersion.data()),
          result.minorVersion.size()) == result.minorVersion.size(),
      error::IsoBmffEarlyStreamEnd);

  uint64_t remainingSize = *result.boxHeader.size - 8;
  Brand brand;
  SPECTRUM_ENFORCE_IF_NOT(remainingSize % brand.size() == 0);
  while (remainingSize >= brand.size()) {
    SPECTRUM_ERROR_IF_NOT(
        _source.read(reinterpret_cast<char*>(brand.data()), brand.size()) ==
            brand.size(),
        error::IsoBmffEarlyStreamEnd);

    result.compatibleBrands.push_back(brand);
    remainingSize -= brand.size();
  }
  result.compatibleBrands.shrink_to_fit();

  return result;
}

} // namespace isobmff
} // namespace codecs
} // namespace spectrum
} // namespace facebook
