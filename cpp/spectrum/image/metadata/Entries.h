// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/DataRange.h>
#include <spectrum/image/Orientation.h>
#include <spectrum/image/metadata/Entry.h>
#include <spectrum/image/metadata/ReadContext.h>

#include <folly/Optional.h>
#include <folly/Range.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace error {
extern const folly::StringPiece InvalidExifHeader;
extern const folly::StringPiece ExifLengthTooLarge;
} // namespace error

class Entries {
  Entry::TagMap _tiff;
  Entry::TagMap _exif;
  Entry::TagMap _gps;

  static constexpr std::uint32_t MAX_DATA_LENGTH = 64 * 1024;

  void _clear();

 public:
  struct MemoryLayout {
    static constexpr const char* HEADER_VALUE = "Exif";
    static constexpr std::uint16_t ENDIAN_CODE_LITTLE = 0x4949;
    static constexpr std::uint16_t ENDIAN_CODE_BIG = 0x4d4d;
    static constexpr std::uint16_t FIXED_VALUE = 0x2a;
    static constexpr std::uint32_t DEFAULT_OFFSET = 8;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static constexpr std::uint16_t ENDIAN_CODE_CURRENT = ENDIAN_CODE_LITTLE;
#else
    static constexpr std::uint16_t ENDIAN_CODE_CURRENT = ENDIAN_CODE_BIG;
#endif

   private:
    const char _header[5];
    const std::uint8_t _padding;
    const std::uint16_t _endianness;
    const std::uint16_t _fixedValue;
    const std::uint32_t _firstIfdOffset;

   public:
    MemoryLayout(
        const char* const header = HEADER_VALUE,
        const std::uint16_t endianness = ENDIAN_CODE_CURRENT,
        const std::uint16_t fixedValue = FIXED_VALUE,
        const std::uint32_t firstIfdOffset = DEFAULT_OFFSET);

    void ensureExpectedLayout(const std::size_t dataLength) const;
    bool littleEndianEncoded() const;
    const std::uint8_t* tiffHeaderBegin() const;
    std::uint32_t firstIfdOffset() const;
  } __attribute__((packed));

  static_assert(
      sizeof(MemoryLayout) == 14,
      "Unexpected Entries::MemoryLayout size");

  Entries(const std::vector<core::DataRange>& dataRanges);

  Entries(const void* const address, const std::size_t dataLength);
  Entries(
      const Entry::TagMap& tiffEntries = {},
      const Entry::TagMap& exifEntries = {},
      const Entry::TagMap& gpsEntries = {});

  void read(const void* const address, const std::size_t dataLength);
  std::vector<std::uint8_t> makeData() const;

  std::size_t entriesSize() const;
  bool empty() const;
  const Entry::TagMap& tiff() const;
  const Entry::TagMap& exif() const;
  const Entry::TagMap& gps() const;

  folly::Optional<image::Orientation> orientation() const;
  void setOrientation(const folly::Optional<image::Orientation>& orientation);

  /**
   * Merges other with this. Values in `other` will override values in self if
   * both are present.
   *
   * @param other the other metadata to merge.
   */
  void merge(const Entries& other);

  bool operator==(const Entries& rhs) const;

 private:
  std::size_t expectedCountOfTiffEntries() const;
};

} // namespace metadata
} // namespace base
} // namespace spectrum
} // namespace facebook
