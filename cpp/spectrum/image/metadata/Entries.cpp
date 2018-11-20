// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Entries.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/utils/Numeric.h>
#include <spectrum/image/metadata/Utils.h>

#include <algorithm>
#include <cstddef>
#include <cstring>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {

namespace error {
const folly::StringPiece InvalidExifHeader{"metadata_invalid_exif_header"};
const folly::StringPiece ExifLengthTooLarge{
    "metadata_invalid_exif_length_too_large"};
} // namespace error

namespace {
void parseEntriesFromTagOffsetIntoTagMap(
    const ReadContext& context,
    const Entry::Tag tag,
    const Entry::TagMap& sourceEntries,
    Entry::TagMap& entriesDestination) {
  const auto iterator = sourceEntries.find(tag);
  if (iterator != sourceEntries.end()) {
    // This try-catch needs to primarily catch potential exceptions thrown in
    // `valueAsLong` to not stop the execution and prevent from parsing further
    // values.
    try {
      const std::uint8_t* const entryBegin =
          context.tiffHeaderBegin + iterator->second.valueAsLong();
      Entry::parseFromAddressIntoTagMap(
          context, entryBegin, entriesDestination);
    } catch (const SpectrumException& e) {
      // TODO T30954248: Warn
      // Unexpected type or value for offset. Silently skipping this entry.
    }
  }
}

void insertSecondaryEntriesIntoData(
    const Entry::TagMap& entries,
    std::vector<std::uint8_t>& data,
    const std::uint32_t currentIDFOffset) {
  if (entries.empty()) {
    return;
  }

  // Insert entries count
  const std::uint16_t entriesCount = entries.size();
  utils::insertValueIntoData(entriesCount, data);

  std::vector<std::uint8_t> valuesData;
  const std::uint32_t valuesOffset = currentIDFOffset + sizeof(std::uint16_t) +
      static_cast<std::uint32_t>(entries.size()) * sizeof(Entry::MemoryLayout) +
      sizeof(std::uint32_t);

  for (const auto& pair : entries) {
    pair.second.insertIntoDataAndValuesData(data, valuesData, valuesOffset);
  }

  // Adds Next IFD offset
  utils::insertValueIntoData(std::uint32_t{0}, data);

  // Insert values after all entries and offset
  data.insert(data.end(), valuesData.begin(), valuesData.end());
}
} // namespace

//
// MemoryLayout
//

Entries::MemoryLayout::MemoryLayout(
    const char* const header,
    const std::uint16_t endianness,
    const std::uint16_t fixedValue,
    const std::uint32_t firstIfdOffset)
    : _header{'\0', '\0', '\0', '\0', '\0'},
      _padding(0),
      _endianness(endianness),
      _fixedValue(fixedValue),
      _firstIfdOffset(firstIfdOffset) {
  SPECTRUM_ENFORCE_IF_NOT(header != nullptr);
  // I really want header to be const, but we need to do this because g++
  // doesn't support literal array initialisation.
  std::strncpy(
      const_cast<char*>(this->_header),
      header,
      std::min(strlen(header) + 1, sizeof(this->_header)));
}

void Entries::MemoryLayout::ensureExpectedLayout(
    const std::size_t dataLength) const {
  SPECTRUM_ERROR_IF(dataLength > MAX_DATA_LENGTH, error::ExifLengthTooLarge);
  SPECTRUM_ERROR_IF(
      dataLength < sizeof(MemoryLayout), error::InvalidExifHeader);
  SPECTRUM_ERROR_IF(
      strncmp(_header, HEADER_VALUE, strlen(HEADER_VALUE) + 1) != 0,
      error::InvalidExifHeader);
  SPECTRUM_ERROR_IF(
      _endianness != ENDIAN_CODE_LITTLE && _endianness != ENDIAN_CODE_BIG,
      error::InvalidExifHeader);
  SPECTRUM_ERROR_IF(
      core::utils::convertValueToNativeByteOrder(
          _fixedValue, littleEndianEncoded()) != FIXED_VALUE,
      error::InvalidExifHeader);
  SPECTRUM_ERROR_IF(
      tiffHeaderBegin() + firstIfdOffset() <
          reinterpret_cast<const std::uint8_t*>(this + 1),
      error::InvalidExifHeader);

  (void)_padding;
}

bool Entries::MemoryLayout::littleEndianEncoded() const {
  return _endianness == ENDIAN_CODE_LITTLE;
}

const std::uint8_t* Entries::MemoryLayout::tiffHeaderBegin() const {
  return reinterpret_cast<const std::uint8_t*>(&_endianness);
}

std::uint32_t Entries::MemoryLayout::firstIfdOffset() const {
  return core::utils::convertValueToNativeByteOrder(
      _firstIfdOffset, littleEndianEncoded());
}

//
// Entries
//

Entries::Entries(const std::vector<core::DataRange>& dataRanges) {
  for (const auto& dataRange : dataRanges) {
    try {
      read(dataRange.begin, dataRange.length);
      break; // we bail after the first successful parse
    } catch (const SpectrumException& e) {
      // do nothing, try next one.
      _clear();
    }
  }
}

Entries::Entries(
    const Entry::TagMap& tiff,
    const Entry::TagMap& exif,
    const Entry::TagMap& gps)
    : _tiff(tiff), _exif(exif), _gps(gps) {}

Entries::Entries(const void* const address, const std::size_t dataLength) {
  read(address, dataLength);
}

void Entries::read(const void* const address, const std::size_t dataLength) {
  SPECTRUM_ENFORCE_IF(dataLength < sizeof(MemoryLayout));
  SPECTRUM_ENFORCE_IF(dataLength > MAX_DATA_LENGTH);

  const auto& layout = *reinterpret_cast<const MemoryLayout*>(address);
  layout.ensureExpectedLayout(dataLength);

  const ReadContext context(
      reinterpret_cast<const std::uint8_t*>(&layout),
      dataLength,
      layout.tiffHeaderBegin(),
      layout.littleEndianEncoded());

  Entry::parseFromAddressIntoTagMap(
      context, context.tiffHeaderBegin + layout.firstIfdOffset(), _tiff);
  parseEntriesFromTagOffsetIntoTagMap(
      context, Entry::EXIF_IFD_POINTER, _tiff, _exif);
  parseEntriesFromTagOffsetIntoTagMap(
      context, Entry::GPS_INFO_IFD_POINTER, _tiff, _gps);
}

std::size_t Entries::entriesSize() const {
  return _tiff.size() + _exif.size() + _gps.size();
}

bool Entries::empty() const {
  return _tiff.empty() && _exif.empty() && _gps.empty();
}

const Entry::TagMap& Entries::tiff() const {
  return _tiff;
}

const Entry::TagMap& Entries::exif() const {
  return _exif;
}

const Entry::TagMap& Entries::gps() const {
  return _gps;
}

void Entries::merge(const Entries& other) {
  for (const auto& pair : other.tiff()) {
    _tiff.erase(pair.first);
    _tiff.insert(pair);
  }

  for (const auto& pair : other.exif()) {
    _exif.erase(pair.first);
    _exif.insert(pair);
  }

  for (const auto& pair : other.gps()) {
    _gps.erase(pair.first);
    _gps.insert(pair);
  }
}

void Entries::_clear() {
  _exif.clear();
  _tiff.clear();
  _gps.clear();
}

//
// Orientation
//

folly::Optional<image::Orientation> Entries::orientation() const {
  const auto iterator = _tiff.find(Entry::ORIENTATION);

  if (iterator != _tiff.end()) {
    const auto exifOrientationValue = iterator->second.valueAsShortOrLong();
    try {
      return image::orientationFromValue(exifOrientationValue);
    } catch (const SpectrumException&) {
    }
  }

  return folly::none;
}

void Entries::setOrientation(
    const folly::Optional<image::Orientation>& orientation) {
  if (orientation.hasValue()) {
    auto iterator = _tiff.find(Entry::ORIENTATION);

    if (iterator != _tiff.end()) {
      iterator->second.setShortOrLongValue(
          static_cast<std::uint32_t>(orientation.value()));
    } else {
      _tiff.insert(std::make_pair(
          Entry::ORIENTATION,
          Entry(
              Entry::ORIENTATION,
              Entry::SHORT,
              static_cast<std::uint16_t>(orientation.value()))));
    }
  } else {
    _tiff.erase(Entry::ORIENTATION);
  }
}

//
// Writing
//

std::size_t Entries::expectedCountOfTiffEntries() const {
  std::size_t count = _tiff.size();

  if (_tiff.find(Entry::EXIF_IFD_POINTER) != _tiff.end()) {
    --count;
  }

  if (_tiff.find(Entry::GPS_INFO_IFD_POINTER) != _tiff.end()) {
    --count;
  }

  const bool shouldContainExifOffset = !_exif.empty();
  const bool shouldContainGpsOffset = !_gps.empty();
  return count + (shouldContainExifOffset ? 1 : 0) +
      (shouldContainGpsOffset ? 1 : 0);
}

std::vector<std::uint8_t> Entries::makeData() const {
  const std::uint16_t count = expectedCountOfTiffEntries();
  const std::uint32_t tiffIfdValuesOffset = MemoryLayout::DEFAULT_OFFSET +
      sizeof(std::uint16_t) + count * sizeof(Entry::MemoryLayout) +
      sizeof(std::uint32_t);

  std::vector<std::uint8_t> data = utils::makeDataFromValue(MemoryLayout());
  std::vector<std::uint8_t> trailingData;

  // In reality data will need a bit more space as it also contains the trailing
  // variable-length values.
  data.reserve(
      (count + _exif.size() + _gps.size()) * sizeof(Entry::MemoryLayout));

  // Add tiff entries count to data.
  utils::insertValueIntoData(count, data);

  for (const auto& pair : _tiff) {
    if (pair.first != Entry::EXIF_IFD_POINTER &&
        pair.first != Entry::GPS_INFO_IFD_POINTER) {
      pair.second.insertIntoDataAndValuesData(
          data, trailingData, tiffIfdValuesOffset);
    }
  }

  const std::uint32_t tiffValuesSize =
      SPECTRUM_CONVERT_OR_THROW(trailingData.size(), std::uint32_t);

  // EXIF IFD begins after the tiff values. This only works because both entries
  // that will be conditionally added later (EXIF_IFD & GPS_IFD) have already
  // been accounted for in tiffIfdValuesOffset *and* we're sure both entry's
  // values fit within an entry's fixed size.
  const std::uint32_t exifIfdOffset = tiffIfdValuesOffset +
      SPECTRUM_CONVERT_OR_THROW(trailingData.size(), std::uint32_t);
  if (!_exif.empty()) {
    Entry(Entry::EXIF_IFD_POINTER, Entry::LONG, exifIfdOffset)
        .insertIntoData(data);
  }

  // Add EXIF entries to trailingData. They cannot be added to data directly as
  // the GPS IFD offset entry may be added later on if needed.
  insertSecondaryEntriesIntoData(_exif, trailingData, exifIfdOffset);

  const std::uint32_t gpsIfdOffset = exifIfdOffset +
      SPECTRUM_CONVERT_OR_THROW(trailingData.size(), std::uint32_t) -
      tiffValuesSize;
  if (!_gps.empty()) {
    Entry(Entry::GPS_INFO_IFD_POINTER, Entry::LONG, gpsIfdOffset)
        .insertIntoData(data);
  }

  // Adds Next IFD offset
  utils::insertValueIntoData(std::uint32_t{0}, data);

  // By this point we won't be adding any more tiff entries so we can safely
  // append the TIFF values + EXIF entries & values at the end of data.
  data.insert(data.end(), trailingData.begin(), trailingData.end());

  // And finally append both GPS entries & values if needed.
  insertSecondaryEntriesIntoData(_gps, data, gpsIfdOffset);

  SPECTRUM_ERROR_IF(data.size() > MAX_DATA_LENGTH, error::ExifLengthTooLarge);
  return data;
}

bool Entries::operator==(const Entries& rhs) const {
  return std::tie(_tiff, _exif, _gps) ==
      std::tie(rhs._tiff, rhs._exif, rhs._gps);
}

} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
