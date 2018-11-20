// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <ImageIO/ImageIO.h>
#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPImageMetadata.h>
#import <SpectrumKit/FSPImageMetadata_Private.h>

using namespace facebook::spectrum::image;
using namespace facebook::spectrum::image::metadata;

@interface FSPImageMetadataTests : XCTestCase
@end

@implementation FSPImageMetadataTests

- (void)testNoneIsReturnedOnEmpty
{
  const auto metadata = [FSPImageMetadata imageMetadataWithDictionary:@{
    (NSString *)kCGImagePropertyTIFFDictionary: @{},
    (NSString *)kCGImagePropertyExifDictionary: @{},
    (NSString *)kCGImagePropertyGPSDictionary: @{},
  }];

  XCTAssertTrue([metadata makeInternalMetadata] == Metadata());
}

- (void)testStringValue
{
  const auto entries = [self testValuesWithMetadataDictionary:@{
    (NSString *)kCGImagePropertyTIFFDictionary: @{
      (NSString *)kCGImagePropertyTIFFMake: @"Facebook",
    },
  }];

  const auto tiffEntries = entries.tiff();
  const auto makeValue = tiffEntries.at(Entry::MAKE).valueAsAsciiString();

  XCTAssertEqual(makeValue, "Facebook");
}

- (void)testByteValue
{
  const auto entries = [self testValuesWithMetadataDictionary:@{
    (NSString *)kCGImagePropertyGPSDictionary: @{
      (NSString *)kCGImagePropertyGPSVersion: @CHAR_MAX,
    },
  }];

  const auto gpsEntries = entries.gps();
  const auto gpsVersion = gpsEntries.at(Entry::GPS_VERSION_ID).valueAsByte();

  XCTAssertEqual(gpsVersion, CHAR_MAX);
}

- (void)testShortValue
{
  const auto entries = [self testValuesWithMetadataDictionary:@{
    (NSString *)kCGImagePropertyTIFFDictionary: @{
      (NSString *)kCGImagePropertyTIFFOrientation: @SHRT_MAX,
    },
  }];

  const auto tiffEntries = entries.tiff();
  const auto orientationValue = tiffEntries.at(Entry::ORIENTATION).valueAsShort();

  XCTAssertEqual(orientationValue, SHRT_MAX);
}

- (void)testLongValue
{
  const auto entries = [self testValuesWithMetadataDictionary:@{
    (NSString *)kCGImagePropertyExifDictionary: @{
      (NSString *)kCGImagePropertyExifISOSpeed: @UINT_MAX,
    },
  }];

  const auto exifEntries = entries.exif();
  const auto isoSpeed = exifEntries.at(Entry::ISO_SPEED).valueAsLong();

  XCTAssertEqual(isoSpeed, UINT_MAX);
}

- (void)testRationalValue
{
  const auto entries = [self testValuesWithMetadataDictionary:@{
    (NSString *)kCGImagePropertyExifDictionary: @{
      (NSString *)kCGImagePropertyExifGamma: @42.43,
    },
  }];

  const auto exifEntries = entries.exif();
  const auto gamma = exifEntries.at(Entry::GAMMA).valueAsRational();

  XCTAssertEqual(gamma.numerator, std::uint32_t{4238034007});
  XCTAssertEqual(gamma.denominator, std::uint32_t{99882960});
}

- (void)testSRationalValue
{
  const auto entries = [self testValuesWithMetadataDictionary:@{
    (NSString *)kCGImagePropertyExifDictionary: @{
      (NSString *)kCGImagePropertyExifShutterSpeedValue: @-42.43,
    },
  }];

  const auto exifEntries = entries.exif();
  const auto shutterSpeed = exifEntries.at(Entry::SHUTTER_SPEED_VALUE).valueAsSRational();

  XCTAssertEqual(shutterSpeed.numerator, -2119017004);
  XCTAssertEqual(shutterSpeed.denominator, 49941480);
}

#pragma mark - Conversion

- (void)testConversionLowestDecimalToUnsignedRational
{
  const auto value = std::numeric_limits<std::uint32_t>::lowest() + (1.0 / 3.0);
  const auto rational = FSPRationalFromValue<std::uint32_t>(@(value));

  XCTAssertLessThanOrEqual(std::abs(rational.value() - value), std::numeric_limits<double>::epsilon());
}

- (void)testConversionHighestDecimalToUnsignedRational
{
  const auto value = std::numeric_limits<std::uint32_t>::max() - 1 + (1.0 / 3.0);
  const auto rational = FSPRationalFromValue<std::uint32_t>(@(value));

  XCTAssertEqual(rational.value(), std::numeric_limits<std::uint32_t>::max() - 1);
}

- (void)testConversionLowestDecimalToSignedRational
{
  const auto value = std::numeric_limits<int32_t>::lowest() + (1.0 / 3.0);
  const auto rational = FSPRationalFromValue<int32_t>(@(value));

  XCTAssertEqual(rational.value(), std::numeric_limits<int32_t>::lowest() + 1);
}

- (void)testConversionHighestDecimalToSignedRational
{
  const auto value = std::numeric_limits<int32_t>::max() - 1 + (1.0 / 3.0);
  const auto rational = FSPRationalFromValue<int32_t>(@(value));

  XCTAssertEqual(rational.value(), std::numeric_limits<int32_t>::max() - 1);
}

- (void)testConversionBoundsChecks
{
  XCTAssertEqual(FSPRationalFromValue<int32_t>(@0).value(), 0.0);
  XCTAssertEqual(FSPRationalFromValue<std::uint32_t>(@0).value(), 0.0);

  XCTAssertEqual(FSPRationalFromValue<int32_t>(@(std::numeric_limits<double>::min())).value(), 0.0);
  XCTAssertEqual(FSPRationalFromValue<std::uint32_t>(@(std::numeric_limits<double>::min())).value(), 0.0);

  XCTAssertEqual(FSPRationalFromValue<int32_t>(@(-std::numeric_limits<double>::min())).value(), 0.0);
  XCTAssertEqual(FSPRationalFromValue<std::uint32_t>(@(-std::numeric_limits<double>::min())).value(), 0.0);
}

- (void)testConversionOfUnsignedInts
{
  auto result = FSPInternalValuesFromNumberValues<std::uint32_t>(@[@1, @3, @42]);
  XCTAssertEqual(result, std::vector<std::uint32_t>({1, 3, 42}));

  result = FSPInternalValuesFromNumberValues<std::uint32_t>(@42);
  XCTAssertEqual(result, std::vector<std::uint32_t>({42}));
}

- (void)testConversionOfSignedInts
{
  auto result = FSPInternalValuesFromNumberValues<int32_t>(@[@-1, @3, @-42]);
  XCTAssertEqual(result, std::vector<int32_t>({-1, 3, -42}));

  result = FSPInternalValuesFromNumberValues<int32_t>(@-42);
  XCTAssertEqual(result, std::vector<int32_t>({-42}));
}

- (void)testConversionOfLatitude
{
  const auto result = FBInternalRationalsFromDecimalAngleValue<std::uint32_t>(@144.9410933);
  const auto expectedResult = std::vector<Rational>{{144, 1}, {56, 1}, {2793, 100}};
  // value = (144 / 1) + ((56 / 1) / 60) + ((2793 / 100) / 3600)

  XCTAssertTrue(result == expectedResult);
}

#pragma mark - Copy

- (void)testCopyIsEqual
{
  const auto object = [FSPImageMetadata imageMetadataWithDictionary:@{}];

  XCTAssertEqualObjects(object, [object copy]);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = [FSPImageMetadata imageMetadataWithDictionary:@{}];
  const auto object2 = [FSPImageMetadata imageMetadataWithDictionary:@{}];

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentDictionary
{
  const auto object = [FSPImageMetadata imageMetadataWithDictionary:@{}];
  const auto object2 = FSPMakeDefaultMetadata();

  XCTAssertNotEqualObjects(object, object2);
}

#pragma mark - Helpers

- (Entries)testValuesWithMetadataDictionary:(NSDictionary<NSString *, id> *)metadataDictionary
{
  const auto metadata = [FSPImageMetadata imageMetadataWithDictionary:metadataDictionary];
  return [metadata makeInternalMetadata].entries();
}

static FSPImageMetadata *FSPMakeDefaultMetadata()
{
  const auto metadataDictionary = @{
    (NSString *)kCGImagePropertyTIFFDictionary: @{
        (NSString *)kCGImagePropertyTIFFMake: @"Facebook",
    },
    (NSString *)kCGImagePropertyExifDictionary: @{},
    (NSString *)kCGImagePropertyGPSDictionary: @{},
  };

  return [FSPImageMetadata imageMetadataWithDictionary:metadataDictionary];
}

@end
