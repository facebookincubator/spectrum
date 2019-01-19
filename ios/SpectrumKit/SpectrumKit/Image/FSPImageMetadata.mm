// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPImageMetadata.h"
#import "FSPImageMetadata_Private.h"

#include <limits>

#import <ImageIO/ImageIO.h>

#import "FSPLog.h"
#include <spectrum/image/metadata/Entries.h>

#import "FSPPrivate.h"

using namespace facebook::spectrum::image;
using namespace facebook::spectrum::image::metadata;

static constexpr auto FSPUnitResolutionDefaultValue = 72;
static constexpr auto FSPColorProfileSRGBValue = 1;

static std::vector<std::uint8_t> dataToVector(NSData *data)
{
  if (data == nil) {
    return {};
  } else {
    const std::uint8_t *dataBegin = static_cast<const std::uint8_t *>(data.bytes);
    const std::uint8_t *dataEnd = dataBegin + data.length;
    return {dataBegin, dataEnd};
  }
}

@implementation FSPImageMetadata {
  NSData *_colorProfileData;
}

#pragma mark - Initialisers

+ (instancetype)imageMetadataWithDictionary:(nullable NSDictionary<NSString *, id> *)dictionary
{
  return [[self alloc] initWithMetadataDictionary:dictionary];
}

+ (instancetype)imageMetadataFromImage:(UIImage *)image
{
  FSPImageMetadata *metadata = [FSPImageMetadata new];

  [metadata addMetadataFromCGImage:image.CGImage];

  return metadata;
}

- (instancetype)initWithMetadataDictionary:(nullable NSDictionary<NSString *, id> *)dictionary
{
  if (self = [super init]) {
    _dictionary = [NSDictionary dictionaryWithDictionary:dictionary];
  }

  return self;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithMetadataDictionary:_dictionary];
}

#pragma mark - Equality

- (BOOL)isEqualToMetadata:(FSPImageMetadata *)object
{
  if (object == nil) {
    return NO;
  }

  return FSPObjectEqualObject(_dictionary, object.dictionary);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPImageMetadata class]]) {
    return [self isEqualToMetadata:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _dictionary.hash;
}

#pragma mark - Internal

- (Metadata)makeInternalMetadata
{
  const auto mappingDictionary = FSPMakeMappingDictionary();
  const auto exifTagMap = FSPInternalTagMap(_dictionary[(NSString *)kCGImagePropertyExifDictionary], mappingDictionary);
  const auto tiffTagMap = FSPInternalTagMap(_dictionary[(NSString *)kCGImagePropertyTIFFDictionary], mappingDictionary);
  const auto gpsTagMap = FSPInternalTagMap(_dictionary[(NSString *)kCGImagePropertyGPSDictionary], mappingDictionary);
  Entries entries(tiffTagMap, exifTagMap, gpsTagMap);
  return Metadata{std::move(entries), ICCProfile{dataToVector(_colorProfileData)}};
}

- (void)addMetadataFromCGImage:(CGImage *)image
{
  NSMutableDictionary<NSString *, id> *const mutableDictionary = [NSMutableDictionary dictionaryWithDictionary:_dictionary];

  {
    // TIFF
    NSMutableDictionary<NSString *, id> *const mutableTIFFDictionary = [NSMutableDictionary dictionaryWithDictionary:mutableDictionary[(NSString *)kCGImagePropertyTIFFDictionary]];
    if (mutableTIFFDictionary[(NSString *)kCGImagePropertyTIFFXResolution] == nil) {
      mutableTIFFDictionary[(NSString*)kCGImagePropertyTIFFXResolution] = @(FSPUnitResolutionDefaultValue);
    }

    if (mutableTIFFDictionary[(NSString *)kCGImagePropertyTIFFYResolution] == nil) {
      mutableTIFFDictionary[(NSString*)kCGImagePropertyTIFFYResolution] = @(FSPUnitResolutionDefaultValue);
    }

    mutableDictionary[(NSString *)kCGImagePropertyTIFFDictionary] = mutableTIFFDictionary;
  }

  {
    // EXIF
    NSMutableDictionary<NSString *, id> *const mutableExifDictionary = [NSMutableDictionary dictionaryWithDictionary:mutableDictionary[(NSString *)kCGImagePropertyExifDictionary]];

    if (@available(iOS 10.0, *)) {
      const CGColorSpaceRef colorSpace = CGImageGetColorSpace(image);
      NSString *const colorSpaceName = (__bridge id)CGColorSpaceCopyName(colorSpace);

      if ([colorSpaceName isEqualToString:(NSString *)kCGColorSpaceSRGB]) {
        mutableExifDictionary[(NSString*)kCGImagePropertyExifColorSpace] = @(FSPColorProfileSRGBValue);
        _colorProfileData = nil;
      } else {
        mutableExifDictionary[(NSString*)kCGImagePropertyExifColorSpace] = nil;
        _colorProfileData = (__bridge id)CGColorSpaceCopyICCData(colorSpace);
      }
    } else {
      // Force sRGB before iOS 10.
      mutableExifDictionary[(NSString*)kCGImagePropertyExifColorSpace] = @(FSPColorProfileSRGBValue);
      _colorProfileData = nil;
    }

    mutableDictionary[(NSString *)kCGImagePropertyExifDictionary] = mutableExifDictionary;
  }

  _dictionary = [mutableDictionary copy];
}

#pragma mark - Helpers

static Entry::TagMap FSPInternalTagMap(NSDictionary<NSString *, id> *const dictionary,
                                             NSDictionary<NSString *, NSArray<NSNumber *> *> *mappingDictionary)
{
  Entry::TagMap map;

  for (NSString *const key in dictionary) {
    const auto mapping = mappingDictionary[key];
    if (mapping != nil) {
      const auto entry = FSPInternalEntry(mapping, dictionary[key]);
      if (entry.hasValue()) {
        map.insert(std::make_pair(entry->getTag(), *entry));
      }
    }
  }

  return map;
}

static folly::Optional<Entry> FSPInternalEntry(NSArray<NSNumber *> *mapping, id value)
{
  const auto tag = static_cast<Entry::Tag>([mapping.firstObject unsignedShortValue]);
  const auto type = static_cast<Entry::Type>([mapping.lastObject unsignedIntegerValue]);

  switch (type) {
    case facebook::spectrum::image::metadata::Entry::BYTE:
    case facebook::spectrum::image::metadata::Entry::UNDEFINED:
      return FSPEntryWithTagTypeAndUntypedValue<std::uint8_t>(tag, type, value);
    case facebook::spectrum::image::metadata::Entry::ASCII:
      return FSPEntryWithTagTypeAndUntypedValue<char>(tag, type, value);
    case facebook::spectrum::image::metadata::Entry::SHORT:
      return FSPEntryWithTagTypeAndUntypedValue<std::uint16_t>(tag, type, value);
    case facebook::spectrum::image::metadata::Entry::LONG:
      return FSPEntryWithTagTypeAndUntypedValue<std::uint32_t>(tag, type, value);
    case facebook::spectrum::image::metadata::Entry::RATIONAL:
      return FSPEntryWithTagTypeAndUntypedValue<Rational>(tag, type, value);
    case facebook::spectrum::image::metadata::Entry::SLONG:
      return FSPEntryWithTagTypeAndUntypedValue<int32_t>(tag, type, value);
    case facebook::spectrum::image::metadata::Entry::SRATIONAL:
      return FSPEntryWithTagTypeAndUntypedValue<SRational>(tag, type, value);
  }
}

static NSDictionary<NSString *, NSArray<NSNumber *> *> *FSPMakeMappingDictionary()
{
  NSMutableDictionary<NSString *, NSArray<NSNumber *> *> *const mappings = [@{
    (NSString *)kCGImagePropertyTIFFImageDescription : @[ @(Entry::Tag::IMAGE_DESCRIPTION), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyTIFFMake : @[ @(Entry::MAKE), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyTIFFModel : @[ @(Entry::MODEL), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyTIFFOrientation : @[ @(Entry::ORIENTATION), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyTIFFXResolution : @[ @(Entry::X_RESOLUTION), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyTIFFYResolution : @[ @(Entry::Y_RESOLUTION), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyTIFFResolutionUnit : @[ @(Entry::RESOLUTION_UNIT), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyTIFFTransferFunction : @[ @(Entry::TRANSFER_FUNCTION), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyTIFFSoftware : @[ @(Entry::SOFTWARE), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyTIFFDateTime : @[ @(Entry::DATE_TIME), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyTIFFArtist : @[ @(Entry::ARTIST), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyTIFFWhitePoint : @[ @(Entry::WHITE_POINT), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyTIFFPrimaryChromaticities : @[ @(Entry::PRIMARY_CHROMATICITIES), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyTIFFCopyright : @[ @(Entry::COPYRIGHT), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifExposureTime : @[ @(Entry::EXPOSURE_TIME), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifFNumber : @[ @(Entry::F_NUMBER), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifExposureProgram : @[ @(Entry::EXPOSURE_PROGRAM), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifSpectralSensitivity : @[ @(Entry::SPECTRAL_SENSITIVITY), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifISOSpeedRatings : @[ @(Entry::ISO_SPEED_RATINGS), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifOECF : @[ @(Entry::OECF), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifSensitivityType : @[ @(Entry::SENSITIVITY_TYPE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifStandardOutputSensitivity : @[ @(Entry::STANDARD_OUTPUT_SENSITIVITY), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifRecommendedExposureIndex : @[ @(Entry::RECOMMENDED_EXPOSURE_INDEX), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifISOSpeed : @[ @(Entry::ISO_SPEED), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifISOSpeedLatitudeyyy : @[ @(Entry::ISO_SPEED_LATITUDE_YYY), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifISOSpeedLatitudezzz : @[ @(Entry::ISO_SPEED_LATITUDE_ZZZ), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifVersion : @[ @(Entry::EXIF_VERSION), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifDateTimeOriginal : @[ @(Entry::DATE_TIME_ORIGINAL), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifDateTimeDigitized : @[ @(Entry::DATE_TIME_DIGITIZED), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifComponentsConfiguration : @[ @(Entry::COMPONENTS_CONFIGURATION), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifCompressedBitsPerPixel : @[ @(Entry::COMPRESSED_BITS_PER_PIXEL), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifShutterSpeedValue : @[ @(Entry::SHUTTER_SPEED_VALUE), @(Entry::SRATIONAL) ],
    (NSString *)kCGImagePropertyExifApertureValue : @[ @(Entry::APERTURE_VALUE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifBrightnessValue : @[ @(Entry::BRIGHTNESS_VALUE), @(Entry::SRATIONAL) ],
    (NSString *)kCGImagePropertyExifExposureBiasValue : @[ @(Entry::EXPOSURE_BIAS_VALUE), @(Entry::SRATIONAL) ],
    (NSString *)kCGImagePropertyExifMaxApertureValue : @[ @(Entry::MAX_APERTURE_VALUE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifSubjectDistance : @[ @(Entry::SUBJECT_DISTANCE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifMeteringMode : @[ @(Entry::METERING_MODE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifLightSource : @[ @(Entry::LIGHT_SOURCE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifFlash : @[ @(Entry::FLASH), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifFocalLength : @[ @(Entry::FOCAL_LENGTH), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifSubjectArea : @[ @(Entry::SUBJECT_AREA), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifMakerNote : @[ @(Entry::MAKER_NOTE), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifUserComment : @[ @(Entry::USER_COMMENT), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifSubsecTime : @[ @(Entry::SUB_SEC_TIME), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifSubsecTimeDigitized : @[ @(Entry::SUB_SEC_TIME_DIGITIZED), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifFlashPixVersion : @[ @(Entry::FLASHPIX_VERSION), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifColorSpace : @[ @(Entry::COLOR_SPACE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifPixelXDimension : @[ @(Entry::PIXEL_X_DIMENSION), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifPixelYDimension : @[ @(Entry::PIXEL_Y_DIMENSION), @(Entry::LONG) ],
    (NSString *)kCGImagePropertyExifRelatedSoundFile : @[ @(Entry::RELATED_SOUND_FILE), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifFlashEnergy : @[ @(Entry::FLASH_ENERGY), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifSpatialFrequencyResponse : @[ @(Entry::SPATIAL_FREQUENCY_RESPONSE), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifFocalPlaneXResolution : @[ @(Entry::FOCAL_PLANE_X_RESOLUTION), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifFocalPlaneYResolution : @[ @(Entry::FOCAL_PLANE_Y_RESOLUTION), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifFocalPlaneResolutionUnit : @[ @(Entry::FOCAL_PLANE_RESOLUTION_UNIT), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifSubjectLocation : @[ @(Entry::SUBJECT_LOCATION), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifExposureIndex : @[ @(Entry::EXPOSURE_INDEX), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifSensingMethod : @[ @(Entry::SENSING_METHOD), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifFileSource : @[ @(Entry::FILE_SOURCE), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifSceneType : @[ @(Entry::SCENE_TYPE), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifCFAPattern : @[ @(Entry::CFA_PATTERN), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifCustomRendered : @[ @(Entry::CUSTOM_RENDERED), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifExposureMode : @[ @(Entry::EXPOSURE_MODE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifWhiteBalance : @[ @(Entry::WHITE_BALANCE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifDigitalZoomRatio : @[ @(Entry::DIGITAL_ZOOM_RATIO), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifFocalLenIn35mmFilm : @[ @(Entry::FOCAL_LENGTH_IN35MM_FILM), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifSceneCaptureType : @[ @(Entry::SCENE_CAPTURE_TYPE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifGainControl : @[ @(Entry::GAIN_CONTROL), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifContrast : @[ @(Entry::CONTRAST), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifSaturation : @[ @(Entry::SATURATION), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifSharpness : @[ @(Entry::SHARPNESS), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifDeviceSettingDescription : @[ @(Entry::DEVICE_SETTING_DESCRIPTION), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyExifSubjectDistRange : @[ @(Entry::SUBJECT_DISTANCE_RANGE), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyExifImageUniqueID : @[ @(Entry::IMAGE_UNIQUE_ID), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifCameraOwnerName : @[ @(Entry::CAMERA_OWNER_NAME), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifBodySerialNumber : @[ @(Entry::BODY_SERIAL_NUMBER), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifLensSpecification : @[ @(Entry::LENS_SPECIFICATION), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyExifLensMake : @[ @(Entry::LENS_MAKE), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifLensModel : @[ @(Entry::LENS_MODEL), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifLensSerialNumber : @[ @(Entry::LENS_SERIAL_NUMBER), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyExifGamma : @[ @(Entry::GAMMA), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSVersion : @[ @(Entry::GPS_VERSION_ID), @(Entry::BYTE) ],
    (NSString *)kCGImagePropertyGPSLatitudeRef : @[ @(Entry::GPS_LATITUDE_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSLatitude : @[ @(Entry::GPS_LATITUDE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSLongitudeRef : @[ @(Entry::GPS_LONGITUDE_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSLongitude : @[ @(Entry::GPS_LONGITUDE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSAltitudeRef : @[ @(Entry::GPS_ALTITUDE_REF), @(Entry::BYTE) ],
    (NSString *)kCGImagePropertyGPSAltitude : @[ @(Entry::GPS_ALTITUDE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSTimeStamp : @[ @(Entry::GPS_TIME_STAMP), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSSatellites : @[ @(Entry::GPS_SATELLITES), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSStatus : @[ @(Entry::GPS_STATUS), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSMeasureMode : @[ @(Entry::GPS_MEASURE_MODE), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDOP : @[ @(Entry::GPSDOP), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSSpeedRef : @[ @(Entry::GPS_SPEED_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSSpeed : @[ @(Entry::GPS_SPEED), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSTrackRef : @[ @(Entry::GPS_TRACK_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSTrack : @[ @(Entry::GPS_TRACK), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSImgDirectionRef : @[ @(Entry::GPS_IMG_DIRECTION_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSImgDirection : @[ @(Entry::GPS_IMG_DIRECTION), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSMapDatum : @[ @(Entry::GPS_MAP_DATUM), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDestLatitudeRef : @[ @(Entry::GPS_DEST_LATITUDE_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDestLatitude : @[ @(Entry::GPS_DEST_LATITUDE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSDestLongitudeRef : @[ @(Entry::GPS_DEST_LONGITUDE_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDestLongitude : @[ @(Entry::GPS_DEST_LONGITUDE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSDestBearingRef : @[ @(Entry::GPS_DEST_BEARING_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDestBearing : @[ @(Entry::GPS_DEST_BEARING), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSDestDistanceRef : @[ @(Entry::GPS_DEST_DISTANCE_REF), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDestDistance : @[ @(Entry::GPS_DEST_DISTANCE), @(Entry::RATIONAL) ],
    (NSString *)kCGImagePropertyGPSProcessingMethod : @[ @(Entry::GPS_PROCESSING_METHOD), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyGPSAreaInformation : @[ @(Entry::GPS_AREA_INFORMATION), @(Entry::UNDEFINED) ],
    (NSString *)kCGImagePropertyGPSDateStamp : @[ @(Entry::GPS_DATE_STAMP), @(Entry::ASCII) ],
    (NSString *)kCGImagePropertyGPSDifferental : @[ @(Entry::GPS_DIFFERENTIAL), @(Entry::SHORT) ],
    (NSString *)kCGImagePropertyGPSHPositioningError : @[ @(Entry::GPS_H_POSITIONING_ERROR), @(Entry::RATIONAL) ],
  } mutableCopy];

  if (@available(iOS 10.0, *)) {
    mappings[(NSString *)kCGImagePropertyExifSubsecTimeOriginal] = @[ @(Entry::SUB_SEC_TIME_ORIGINAL), @(Entry::ASCII) ];
  }

  return mappings;
}

#pragma mark - Conversion

/**
 * Transforms NSNumber into T.
 *
 * @param number The number to transform.
 * @return The unwrapped value.
 */
template <typename T>
T FSPNumberToInternalNumber(NSNumber *const number);

/**
 * Returns a std::vector<T> from the value.
 *
 * @param tag The tag related to the current value.
 * @param value The value to encode.
 * @return vector ontaining the serialised values.
 */
template <typename T>
std::vector<T> FSPInternalValueFromValue(const Entry::Tag tag, const id value);


template <typename T>
folly::Optional<Entry> FSPEntryWithTagTypeAndUntypedValue(const Entry::Tag tag,
                                                          const Entry::Type type,
                                                          const id value)
{
  const auto values = FSPInternalValueFromValue<T>(tag, value);
  return values.empty() ? folly::none : folly::make_optional(Entry(tag, type, values));
}

template <typename T>
RationalBase<T> FSPRationalFromValue(NSNumber *const number)
{
  const auto lowest = std::numeric_limits<T>::lowest();
  const auto max = std::numeric_limits<T>::max();

  if (number.doubleValue <= static_cast<double>(lowest)) {
    // We've got a out of bounds value
    return RationalBase<T>{lowest, 1};
  } else if (number.doubleValue >= static_cast<double>(max)) {
    // We've got a out of bounds value
    return RationalBase<T>{max, 1};
  } else if (static_cast<T>(number.doubleValue) == number.doubleValue) {
    // We've got an integral value
    return RationalBase<T>{FSPNumberToInternalNumber<T>(number), 1};
  } else {
    const auto denominatorDenominatorFactor = number.doubleValue < 0 ? std::floor(number.doubleValue) : std::ceil(number.doubleValue);
    const auto denominatorNumeratorFactor = number.doubleValue < 0 ? double{lowest} : double{max};
    const auto denominator = denominatorNumeratorFactor / denominatorDenominatorFactor;
    const auto numerator = denominator * number.doubleValue;

    return {
      static_cast<T>(numerator),
      static_cast<T>(denominator),
    };
  }
}

template <typename T>
std::vector<T> FSPInternalValuesFromNumberValues(const id value)
{
  std::vector<T> internalValues;
  NSArray<NSNumber *> const *arrayValue = value;

  if ([value isKindOfClass:[NSNumber class]]) {
    arrayValue = @[value];
  }

  if ([arrayValue isKindOfClass:[NSArray class]]) {
    for (NSNumber *number in arrayValue) {
      if ([number isKindOfClass:[NSNumber class]]) {
        internalValues.push_back(FSPNumberToInternalNumber<T>(number));
      } else {
        FSPCReportMustFix(@"Unexpected value: %@", value);
        internalValues = {};
        break;
      }
    }
  }

  return internalValues;
}

template <typename T>
std::vector<RationalBase<T>> FBInternalRationalsFromDecimalAngleValue(NSNumber *const number)
{
  double valueDouble = fabs(number.doubleValue);

  const T degrees = static_cast<T>(valueDouble);

  valueDouble -= degrees;
  valueDouble *= 60;

  const T minutes = valueDouble;

  valueDouble -= minutes;
  valueDouble *= 60;

  return {
    {static_cast<T>(degrees), static_cast<T>(1)},
    {static_cast<T>(minutes), static_cast<T>(1)},
    {static_cast<T>(valueDouble * 100.0), static_cast<T>(100)},
  };
}

template <typename T>
std::vector<RationalBase<T>> FSPInternalRationalsFromValues(const id value, const BOOL isDegrees)
{
  if (isDegrees) {
    if ([value isKindOfClass:[NSNumber class]]) {
      return FBInternalRationalsFromDecimalAngleValue<T>(value);
    } else {
      FSPCReportMustFix(@"Unexpected value: %@", value);
      return {};
    }
  } else {
    if ([value isKindOfClass:[NSNumber class]]) {
      return {
        FSPRationalFromValue<T>(value),
      };
    } else if ([value isKindOfClass:[NSArray class]]) {
      NSArray<NSNumber *> *const numbers = value;
      std::vector<RationalBase<T>> rationals;
      for (NSNumber *number in numbers) {
        rationals.push_back(FSPRationalFromValue<T>(number));
      }
      return rationals;
    } else {
      FSPCReportMustFix(@"Unexpected value: %@", value);
      return {};
    }
  }
}

#pragma mark - Value to std::vector<T>

template <>
std::vector<char> FSPInternalValueFromValue(__unused const Entry::Tag tag, const id value)
{
  std::vector<char> internalValues;
  const auto stringValue = (NSString *)value;

  if ([value isKindOfClass:[NSString class]] && stringValue.length > 0) {
    const auto length = [stringValue lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
    internalValues.resize(length + 1);
    std::memcpy(internalValues.data(), stringValue.UTF8String, length + 1);
  }

  return internalValues;
}

template <>
std::vector<Rational> FSPInternalValueFromValue(const Entry::Tag tag, const id value)
{
  const BOOL isDegreesEncoded = tag == Entry::GPS_LONGITUDE || tag == Entry::GPS_LATITUDE || tag == Entry::GPS_TIME_STAMP;
  return FSPInternalRationalsFromValues<Rational::_Value>(value, isDegreesEncoded);
}

template <>
std::vector<SRational> FSPInternalValueFromValue(const Entry::Tag tag, const id value)
{
  const BOOL isDegreesEncoded = tag == Entry::GPS_LONGITUDE || tag == Entry::GPS_LATITUDE || tag == Entry::GPS_TIME_STAMP;
  return FSPInternalRationalsFromValues<SRational::_Value>(value, isDegreesEncoded);
}

template <>
std::vector<std::uint8_t> FSPInternalValueFromValue(__unused const Entry::Tag tag, const id value)
{
  return FSPInternalValuesFromNumberValues<std::uint8_t>(value);
}

template <>
std::vector<std::uint16_t> FSPInternalValueFromValue(__unused const Entry::Tag tag, const id value)
{
  return FSPInternalValuesFromNumberValues<std::uint16_t>(value);
}

template <>
std::vector<std::uint32_t> FSPInternalValueFromValue(__unused const Entry::Tag tag, const id value)
{
  return FSPInternalValuesFromNumberValues<std::uint32_t>(value);
}

template <>
std::vector<int32_t> FSPInternalValueFromValue(__unused const Entry::Tag tag, const id value)
{
  return FSPInternalValuesFromNumberValues<int32_t>(value);
}

#pragma mark - NSNumber Helpers

template <>
std::uint8_t FSPNumberToInternalNumber(NSNumber *const number)
{
  return static_cast<std::uint8_t>(number.unsignedShortValue);
}

template <>
std::uint16_t FSPNumberToInternalNumber(NSNumber *const number)
{
  return number.unsignedShortValue;
}

template <>
int32_t FSPNumberToInternalNumber(NSNumber *const number)
{
  return number.intValue;
}

template <>
std::uint32_t FSPNumberToInternalNumber(NSNumber *const number)
{
  return number.unsignedIntValue;
}

@end
