// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Entry.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/utils/Numeric.h>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <sstream>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace error {
const folly::StringPiece UnexpectedEntryTypeOrCount{
    "metadata_unexpected_entry_type_or_count"};
const folly::StringPiece DataNotLargeEnough{
    "metadata_entry_data_not_large_enough"};
} // namespace error

namespace {
std::string entryTypeStringFromValue(const Entry::Type type) {
  switch (type) {
    case Entry::BYTE:
      return "byte";
    case Entry::ASCII:
      return "ascii";
    case Entry::SHORT:
      return "short";
    case Entry::LONG:
      return "long";
    case Entry::RATIONAL:
      return "rational";
    case Entry::UNDEFINED:
      return "undefined";
    case Entry::SLONG:
      return "slong";
    case Entry::SRATIONAL:
      return "srational";
    default:
      return ::facebook::spectrum::core::makeUnknownWithValue<std::uint32_t>(
          type);
  }
}

std::size_t sizeForType(const Entry::Type type) {
  switch (type) {
    case Entry::BYTE:
      return sizeof(std::uint8_t);
    case Entry::ASCII:
      return sizeof(char);
    case Entry::SHORT:
      return sizeof(std::uint16_t);
    case Entry::LONG:
      return sizeof(std::uint32_t);
    case Entry::RATIONAL:
      return sizeof(Rational);
    case Entry::UNDEFINED:
      return sizeof(std::uint8_t);
    case Entry::SLONG:
      return sizeof(int32_t);
    case Entry::SRATIONAL:
      return sizeof(SRational);
    default:
      SPECTRUM_UNREACHABLE_METADATA_ENTRY_TYPE(type);
  }
}
} // namespace

//
// Memory Layout
//

const std::uint8_t* Entry::MemoryLayout::valuesBeginAddress(
    const ReadContext& context,
    const std::size_t sizeOfElement) const {
  const auto countOfValues = core::utils::convertValueToNativeByteOrder(
      count, context.isLittleEndianEncoded);

  if (countOfValues * sizeOfElement <= sizeof(valueOffset)) {
    // Value(s) fits within `valueOffset`.
    return reinterpret_cast<const std::uint8_t*>(&valueOffset);
  } else {
    // Value(s) are stored at tiff header + `valueOffset`.
    const auto valueOffset = core::utils::convertValueToNativeByteOrder(
        this->valueOffset, context.isLittleEndianEncoded);
    return context.tiffHeaderBegin + valueOffset;
  }
}

template <typename T>
std::vector<std::uint8_t> Entry::MemoryLayout::extractTypedValue(
    const ReadContext& context) const {
  const auto countOfValues = core::utils::convertValueToNativeByteOrder(
      count, context.isLittleEndianEncoded);
  const auto valuesBegin = valuesBeginAddress(context, sizeof(T));
  const auto valuesEnd = valuesBegin + countOfValues * sizeof(T);

  SPECTRUM_ERROR_IF(valuesEnd > context.dataEnd, error::DataNotLargeEnough);

  if (sizeof(T) == 1) {
    // Optimisation: there's no need to reverse the endianness if the type is 1
    // byte-long.
    return std::vector<std::uint8_t>(valuesBegin, valuesEnd);
  } else {
    const auto typedValuesBegin = reinterpret_cast<const T*>(valuesBegin);

    std::vector<std::uint8_t> values;
    values.resize(countOfValues * sizeof(T));

    const auto vectorValuesData = reinterpret_cast<T*>(values.data());
    for (std::uint32_t i = 0; i < countOfValues; ++i) {
      const T value = core::utils::convertValueToNativeByteOrder(
          typedValuesBegin[i], context.isLittleEndianEncoded);
      memcpy(&vectorValuesData[i], &value, sizeof(T));
    }

    return values;
  }
}

std::vector<std::uint8_t> Entry::MemoryLayout::extractValue(
    const ReadContext& context) const {
  const auto type =
      static_cast<Entry::Type>(core::utils::convertValueToNativeByteOrder(
          this->type, context.isLittleEndianEncoded));

  switch (type) {
    case Entry::UNDEFINED:
    case Entry::BYTE:
      return extractTypedValue<std::uint8_t>(context);
    case Entry::ASCII:
      return extractTypedValue<char>(context);
    case Entry::SHORT:
      return extractTypedValue<std::uint16_t>(context);
    case Entry::LONG:
      return extractTypedValue<std::uint32_t>(context);
    case Entry::RATIONAL:
      return extractTypedValue<Rational>(context);
    case Entry::SLONG:
      return extractTypedValue<int32_t>(context);
    case Entry::SRATIONAL:
      return extractTypedValue<SRational>(context);
    default:
      SPECTRUM_UNREACHABLE_METADATA_ENTRY_TYPE(type);
  }
}

//
// Get Value
//

template <typename T>
T Entry::valueAs(const Type expectedType) const {
  SPECTRUM_ERROR_FORMAT_IF_NOT(
      type == expectedType && count == 1,
      error::UnexpectedEntryTypeOrCount,
      "type: %s, expectedType: %s, count: %u, expectedCount: %u",
      entryTypeStringFromValue(type).c_str(),
      entryTypeStringFromValue(expectedType).c_str(),
      count,
      1);
  return *reinterpret_cast<const T*>(value.data());
}

template <typename T>
std::vector<T> Entry::valuesAsVector(const Type expectedType) const {
  SPECTRUM_ERROR_FORMAT_IF_NOT(
      type == expectedType,
      error::UnexpectedEntryTypeOrCount,
      "type: %s, expectedType: %s, count: %u, expectedCount: none",
      entryTypeStringFromValue(type).c_str(),
      entryTypeStringFromValue(expectedType).c_str(),
      count);

  const auto begin = reinterpret_cast<const T*>(value.data());
  return std::vector<T>(begin, begin + count);
}

std::uint8_t Entry::valueAsByte() const {
  return valueAs<std::uint8_t>(Type::BYTE);
}

std::uint16_t Entry::valueAsShort() const {
  return valueAs<std::uint16_t>(Type::SHORT);
}

std::uint32_t Entry::valueAsLong() const {
  return valueAs<std::uint32_t>(Type::LONG);
}

std::uint32_t Entry::valueAsShortOrLong() const {
  if (type == Type::SHORT) {
    return static_cast<std::uint32_t>(valueAs<std::uint16_t>(Type::SHORT));
  } else {
    return valueAs<std::uint32_t>(Type::LONG);
  }
}

Rational Entry::valueAsRational() const {
  return valueAs<Rational>(Type::RATIONAL);
}

SRational Entry::valueAsSRational() const {
  return valueAs<SRational>(Type::SRATIONAL);
}

std::uint8_t Entry::valueAsUndefined() const {
  return valueAs<std::uint8_t>(Type::UNDEFINED);
}

std::vector<std::uint8_t> Entry::valueAsByteVector() const {
  return valuesAsVector<std::uint8_t>(Type::BYTE);
}

std::vector<std::uint16_t> Entry::valueAsShortVector() const {
  return valuesAsVector<std::uint16_t>(Type::SHORT);
}

std::vector<std::uint32_t> Entry::valueAsLongVector() const {
  return valuesAsVector<std::uint32_t>(Type::LONG);
}

std::vector<std::uint32_t> Entry::valueAsShortOrLongVector() const {
  if (type == Type::SHORT) {
    const auto shortVector = valuesAsVector<std::uint16_t>(Type::SHORT);
    return std::vector<std::uint32_t>(shortVector.begin(), shortVector.end());
  } else {
    return valuesAsVector<std::uint32_t>(Type::LONG);
  }
}

std::vector<Rational> Entry::valueAsRationalVector() const {
  return valuesAsVector<Rational>(Entry::RATIONAL);
}

std::vector<std::uint8_t> Entry::valueAsUndefinedVector() const {
  return valuesAsVector<std::uint8_t>(Type::UNDEFINED);
}

std::string Entry::valueAsAsciiString() const {
  SPECTRUM_ERROR_FORMAT_IF_NOT(
      type == Type::ASCII,
      error::UnexpectedEntryTypeOrCount,
      "type: %s, expectedType: %s, count: %u, expectedCount: none",
      entryTypeStringFromValue(type).c_str(),
      entryTypeStringFromValue(Entry::ASCII).c_str(),
      count);

  auto endIterator = value.end();
  while (endIterator != value.begin() && *(endIterator - 1) == '\0') {
    --endIterator;
  }

  return std::string(value.begin(), endIterator);
}

//
// Set Value
//

void Entry::setShortOrLongValue(const std::uint32_t newValue) {
  SPECTRUM_ERROR_FORMAT_IF_NOT(
      (type == Type::SHORT || type == Type::LONG) && count == 1,
      error::UnexpectedEntryTypeOrCount,
      "type: %s, expectedType: %s or %s, count: %u, expectedCount: %u",
      entryTypeStringFromValue(type).c_str(),
      entryTypeStringFromValue(Type::SHORT).c_str(),
      entryTypeStringFromValue(Type::LONG).c_str(),
      count,
      1);

  if (type == Type::SHORT) {
    value = utils::makeDataFromValue(static_cast<std::uint16_t>(newValue));
  } else {
    value = utils::makeDataFromValue(newValue);
  }
}

//
// Serialisation
//

void Entry::insertIntoData(std::vector<std::uint8_t>& data) const {
  SPECTRUM_ENFORCE_IF_NOT(value.size() <= sizeof(MemoryLayout::valueOffset));
  utils::insertValueIntoData(
      MemoryLayout(tag, type, count, utils::make4ByteDataFromValue(value)),
      data);
}

void Entry::insertIntoDataAndValuesData(
    std::vector<std::uint8_t>& data,
    std::vector<std::uint8_t>& valuesData,
    const std::uint32_t initialValuesOffset) const {
  const auto valuesSize = count * sizeForType(type);

  if (valuesSize <= sizeof(MemoryLayout::valueOffset)) {
    // Encode values in valueOffset.
    insertIntoData(data);
  } else {
    // Encode values in valuesData and reference the location in valueOffset.
    const auto currentValuesOffset = initialValuesOffset + valuesData.size();
    valuesData.insert(valuesData.end(), value.begin(), value.end());
    utils::insertValueIntoData(
        MemoryLayout(
            tag,
            type,
            count,
            SPECTRUM_CONVERT_OR_THROW(currentValuesOffset, std::uint32_t)),
        data);

    if (value.size() % 2 == 1) {
      // Add one byte padding to the values in case we're got an odd count.
      valuesData.push_back(0);
    }
  }
}

//
// Deserialisation
//

void Entry::parseFromAddressIntoTagMap(
    const ReadContext& context,
    const std::uint8_t* const begin,
    TagMap& entriesDestination) {
  SPECTRUM_ERROR_IF(
      begin + sizeof(std::uint16_t) > context.dataEnd,
      error::DataNotLargeEnough);

  const auto countOfEntriesAddress =
      reinterpret_cast<const std::uint16_t*>(begin);
  const auto countOfEntries = core::utils::convertValueToNativeByteOrder(
      *countOfEntriesAddress, context.isLittleEndianEncoded);

  SPECTRUM_ERROR_IF(
      begin + sizeof(std::uint16_t) + (countOfEntries * sizeof(MemoryLayout)) >
          context.dataEnd,
      error::DataNotLargeEnough);

  const MemoryLayout* entriesLayout =
      reinterpret_cast<const MemoryLayout*>(countOfEntriesAddress + 1);
  for (std::uint16_t i = 0; i < countOfEntries; ++i) {
    const auto tag =
        static_cast<Tag>(core::utils::convertValueToNativeByteOrder(
            entriesLayout[i].tag, context.isLittleEndianEncoded));
    const auto type =
        static_cast<Type>(core::utils::convertValueToNativeByteOrder(
            entriesLayout[i].type, context.isLittleEndianEncoded));
    const auto count = core::utils::convertValueToNativeByteOrder(
        entriesLayout[i].count, context.isLittleEndianEncoded);
    try {
      entriesDestination.insert(std::make_pair(
          tag,
          Entry(tag, type, count, entriesLayout[i].extractValue(context))));
    } catch (const SpectrumException& e) {
      // TODO T30954248: WARN
    }
  }
}

//
// - Operators
//

bool Entry::operator==(const Entry& other) const {
  return std::tie(tag, type, count, value) ==
      std::tie(other.tag, other.type, other.count, other.value);
}

bool Entry::operator!=(const Entry& other) const {
  return !(*this == other);
}
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
