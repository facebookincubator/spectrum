// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/metadata/Rational.h>
#include <spectrum/image/metadata/ReadContext.h>
#include <spectrum/image/metadata/Utils.h>

#include <folly/Optional.h>
#include <folly/Range.h>

#include <cstddef>
#include <cstdio>
#include <map>
#include <vector>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace error {
extern const folly::StringPiece UnexpectedEntryTypeOrCount;
extern const folly::StringPiece DataNotLargeEnough;
} // namespace error

struct Entry {
  enum Tag : std::uint16_t {
    // Support 1
    IMAGE_DESCRIPTION = 0x10E,
    MAKE = 0x10F,
    MODEL = 0x110,
    ORIENTATION = 0x112,
    X_RESOLUTION = 0x11A,
    Y_RESOLUTION = 0x11B,
    RESOLUTION_UNIT = 0x128,
    TRANSFER_FUNCTION = 0x12D,
    SOFTWARE = 0x131,
    DATE_TIME = 0x132,
    ARTIST = 0x13B,
    WHITE_POINT = 0x13E,
    PRIMARY_CHROMATICITIES = 0x13F,
    Y_CB_CR_COEFFICIENTS = 0x211,
    Y_CB_CR_POSITIONING = 0x213,
    REFERENCE_BLACK_WHITE = 0x214,
    COPYRIGHT = 0x8298,
    EXIF_IFD_POINTER = 0x8769,
    GPS_INFO_IFD_POINTER = 0x8825,

    // Support 2
    EXPOSURE_TIME = 0x829A,
    F_NUMBER = 0x829D,
    EXPOSURE_PROGRAM = 0x8822,
    SPECTRAL_SENSITIVITY = 0x8824,
    ISO_SPEED_RATINGS = 0x8827,
    OECF = 0x8828,
    SENSITIVITY_TYPE = 0x8830,
    STANDARD_OUTPUT_SENSITIVITY = 0x8831,
    RECOMMENDED_EXPOSURE_INDEX = 0x8832,
    ISO_SPEED = 0x8833,
    ISO_SPEED_LATITUDE_YYY = 0x8834,
    ISO_SPEED_LATITUDE_ZZZ = 0x8835,
    EXIF_VERSION = 0x9000,
    DATE_TIME_ORIGINAL = 0x9003,
    DATE_TIME_DIGITIZED = 0x9004,
    COMPONENTS_CONFIGURATION = 0x9101,
    COMPRESSED_BITS_PER_PIXEL = 0x9102,
    SHUTTER_SPEED_VALUE = 0x9201,
    APERTURE_VALUE = 0x9202,
    BRIGHTNESS_VALUE = 0x9203,
    EXPOSURE_BIAS_VALUE = 0x9204,
    MAX_APERTURE_VALUE = 0x9205,
    SUBJECT_DISTANCE = 0x9206,
    METERING_MODE = 0x9207,
    LIGHT_SOURCE = 0x9208,
    FLASH = 0x9209,
    FOCAL_LENGTH = 0x920A,
    SUBJECT_AREA = 0x9214,
    MAKER_NOTE = 0x927C,
    USER_COMMENT = 0x9286,
    SUB_SEC_TIME = 0x9290,
    SUB_SEC_TIME_ORIGINAL = 0x9291,
    SUB_SEC_TIME_DIGITIZED = 0x9292,
    FLASHPIX_VERSION = 0xA000,
    COLOR_SPACE = 0xA001,
    PIXEL_X_DIMENSION = 0xA002,
    PIXEL_Y_DIMENSION = 0xA003,
    RELATED_SOUND_FILE = 0xA004,
    INTEROPERABILITY_IFD_POINTER = 0xA005,
    FLASH_ENERGY = 0xA20B,
    SPATIAL_FREQUENCY_RESPONSE = 0xA20C,
    FOCAL_PLANE_X_RESOLUTION = 0xA20E,
    FOCAL_PLANE_Y_RESOLUTION = 0xA20F,
    FOCAL_PLANE_RESOLUTION_UNIT = 0xA210,
    SUBJECT_LOCATION = 0xA214,
    EXPOSURE_INDEX = 0xA215,
    SENSING_METHOD = 0xA217,
    FILE_SOURCE = 0xA300,
    SCENE_TYPE = 0xA301,
    CFA_PATTERN = 0xA302,
    CUSTOM_RENDERED = 0xA401,
    EXPOSURE_MODE = 0xA402,
    WHITE_BALANCE = 0xA403,
    DIGITAL_ZOOM_RATIO = 0xA404,
    FOCAL_LENGTH_IN35MM_FILM = 0xA405,
    SCENE_CAPTURE_TYPE = 0xA406,
    GAIN_CONTROL = 0xA407,
    CONTRAST = 0xA408,
    SATURATION = 0xA409,
    SHARPNESS = 0xA40A,
    DEVICE_SETTING_DESCRIPTION = 0xA40B,
    SUBJECT_DISTANCE_RANGE = 0xA40C,
    IMAGE_UNIQUE_ID = 0xA420,
    CAMERA_OWNER_NAME = 0xA430,
    BODY_SERIAL_NUMBER = 0xA431,
    LENS_SPECIFICATION = 0xA432,
    LENS_MAKE = 0xA433,
    LENS_MODEL = 0xA434,
    LENS_SERIAL_NUMBER = 0xA435,
    GAMMA = 0xA500,

    // Support 3
    GPS_VERSION_ID = 0x0,
    GPS_LATITUDE_REF = 0x1,
    GPS_LATITUDE = 0x2,
    GPS_LONGITUDE_REF = 0x3,
    GPS_LONGITUDE = 0x4,
    GPS_ALTITUDE_REF = 0x5,
    GPS_ALTITUDE = 0x6,
    GPS_TIME_STAMP = 0x7,
    GPS_SATELLITES = 0x8,
    GPS_STATUS = 0x9,
    GPS_MEASURE_MODE = 0xA,
    GPSDOP = 0xB,
    GPS_SPEED_REF = 0xC,
    GPS_SPEED = 0xD,
    GPS_TRACK_REF = 0xE,
    GPS_TRACK = 0xF,
    GPS_IMG_DIRECTION_REF = 0x10,
    GPS_IMG_DIRECTION = 0x11,
    GPS_MAP_DATUM = 0x12,
    GPS_DEST_LATITUDE_REF = 0x13,
    GPS_DEST_LATITUDE = 0x14,
    GPS_DEST_LONGITUDE_REF = 0x15,
    GPS_DEST_LONGITUDE = 0x16,
    GPS_DEST_BEARING_REF = 0x17,
    GPS_DEST_BEARING = 0x18,
    GPS_DEST_DISTANCE_REF = 0x19,
    GPS_DEST_DISTANCE = 0x1A,
    GPS_PROCESSING_METHOD = 0x1B,
    GPS_AREA_INFORMATION = 0x1C,
    GPS_DATE_STAMP = 0x1D,
    GPS_DIFFERENTIAL = 0x1E,
    GPS_H_POSITIONING_ERROR = 0x1F,
  };

  enum Type : std::uint32_t {
    BYTE = 1,
    ASCII = 2,
    SHORT = 3,
    LONG = 4,
    RATIONAL = 5,
    UNDEFINED = 7,
    SLONG = 8,
    SRATIONAL = 10
  };

  using TagMap = std::map<Entry::Tag, Entry>;

  struct MemoryLayout {
    const std::uint16_t tag;
    const std::uint16_t type;
    const std::uint32_t count;
    const std::uint32_t valueOffset;

    MemoryLayout(
        const std::uint16_t tag,
        const std::uint16_t type,
        const std::uint32_t count,
        const std::uint32_t valueOffset)
        : tag(tag), type(type), count(count), valueOffset(valueOffset) {}

    std::vector<std::uint8_t> extractValue(const ReadContext& context) const;

   private:
    const std::uint8_t* valuesBeginAddress(
        const ReadContext& context,
        const std::size_t sizeOfElement) const;

    template <typename T>
    std::vector<std::uint8_t> extractTypedValue(
        const ReadContext& context) const;

  } __attribute__((packed));

  static_assert(
      sizeof(MemoryLayout) == 12,
      "Unexpected Entry::MemoryLayout size");

  template <typename T>
  Entry(const Tag tag, const Type type, const T& value)
      : tag(tag),
        type(type),
        count(1),
        value(utils::makeDataFromValue(value)) {}

  template <typename T>
  Entry(const Tag tag, const Type type, const std::vector<T>& values)
      : tag(tag),
        type(type),
        count(static_cast<std::uint32_t>(values.size())),
        value(utils::makeDataFromValue(values)) {}

  template <typename T>
  Entry(
      const Tag tag,
      const Type type,
      const std::uint32_t count,
      const T& values)
      : tag(tag),
        type(type),
        count(count),
        value(utils::makeDataFromValue(values)) {}

  Entry(const Entry& other)
      : tag(other.tag),
        type(other.type),
        count(other.count),
        value(other.value) {}

  Tag getTag() const {
    return tag;
  }

  std::uint8_t valueAsByte() const;
  std::uint16_t valueAsShort() const;
  std::uint32_t valueAsLong() const;
  std::uint32_t valueAsShortOrLong() const;
  Rational valueAsRational() const;
  SRational valueAsSRational() const;
  std::uint8_t valueAsUndefined() const;

  std::vector<std::uint8_t> valueAsByteVector() const;
  std::vector<std::uint16_t> valueAsShortVector() const;
  std::vector<std::uint32_t> valueAsLongVector() const;
  std::vector<std::uint32_t> valueAsShortOrLongVector() const;
  std::vector<Rational> valueAsRationalVector() const;
  std::vector<std::uint8_t> valueAsUndefinedVector() const;
  std::string valueAsAsciiString() const;

  void setShortOrLongValue(const std::uint32_t newValue);

  static void parseFromAddressIntoTagMap(
      const ReadContext& context,
      const std::uint8_t* const begin,
      TagMap& entriesDestination);

  void insertIntoData(std::vector<std::uint8_t>& data) const;
  void insertIntoDataAndValuesData(
      std::vector<std::uint8_t>& data,
      std::vector<std::uint8_t>& valuesData,
      const std::uint32_t initialValuesOffset) const;

  bool operator==(const Entry& lhs) const;
  bool operator!=(const Entry& lhs) const;

 private:
  Tag tag;
  Type type;
  std::uint32_t count;
  std::vector<std::uint8_t> value;

  template <typename T>
  T valueAs(const Type expectedType) const;

  template <typename T>
  std::vector<T> valuesAsVector(const Type expectedType) const;
};

#define SPECTRUM_UNREACHABLE_METADATA_ENTRY_TYPE(entryType)  \
  SPECTRUM_UNREACHABLE_STRING(                               \
      folly::StringPiece{"unreachable_metadata_entry_type"}, \
      entryTypeStringFromValue(entryType))

} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
