// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPConfiguration.h>
#import <SpectrumKit/FSPConfiguration_Private.h>

using namespace facebook::spectrum;

@interface FSPConfigurationTests : XCTestCase
@end

@implementation FSPConfigurationTests

- (void)testDefaultValues
{
  const auto configuration = [FSPConfiguration new];

  XCTAssertEqual(configuration.general.interpretMetadata, YES);
  XCTAssertEqualObjects(configuration.general.defaultBackgroundColor, [UIColor colorWithRed:1 green:1 blue:1 alpha:1]);
  XCTAssertEqual(configuration.general.propagateChromaSamplingModeFromSource, YES);
  XCTAssertEqual(configuration.general.chromaSamplingModeOverride, FSPConfigurationChromaSamplingModeOverrideNone);

  XCTAssertEqual(configuration.jpeg.useTrellis, YES);
  XCTAssertEqual(configuration.jpeg.useProgressive, NO);
  XCTAssertEqual(configuration.jpeg.useOptimizeScan, NO);
  XCTAssertEqual(configuration.jpeg.useCompatibleDCScanOption, NO);
  XCTAssertEqual(configuration.jpeg.usePSNRQuantTable, NO);

  XCTAssertEqual(configuration.png.useInterlacing, NO);
  XCTAssertEqual(configuration.png.compressionLevel, FSPPngCompressionLevelDefault);

  XCTAssertEqual(configuration.webp.method, 3);
  XCTAssertEqual(configuration.webp.imageHint, FSPConfigurationWebpImageHintDefault);
}

- (void)testInternalTypeConversion
{
  XCTAssertEqual(makeDefaultInternalConfiguration(), [makeDefaultConfiguration() makeInternalConfiguration]);
}

#pragma mark - Copy

- (void)testCopyIsEqual
{
  const auto object = makeDefaultConfiguration();

  XCTAssertEqualObjects(object, [object copy]);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentGeneralBackgroundColor
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.general.defaultBackgroundColor = UIColor.greenColor;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentGeneralInterpretMetadata
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.general.interpretMetadata = YES;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentGeneralPropagateChromaSamplingModeFromSource
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();
  
  object2.general.propagateChromaSamplingModeFromSource = NO;
  
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentGeneralChromaSamplingModeOverride
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();
  
  object2.general.chromaSamplingModeOverride = FSPConfigurationChromaSamplingModeOverride420;
  
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentJpegUseTrellis
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.jpeg.useTrellis = YES;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentJpegUseProgressive
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.jpeg.useProgressive = NO;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentJpegUseOptimizeScan
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.jpeg.useOptimizeScan = YES;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentJpegUseCompatibleDCScanOption
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.jpeg.useCompatibleDCScanOption = NO;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentJpegUsePSNRQuantTable
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.jpeg.usePSNRQuantTable = NO;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentPngUseInterlacing
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.png.useInterlacing = NO;

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentPngCompressionLevel
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();
    
  object2.png.compressionLevel = FSPPngCompressionLevelBestSpeed;
    
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentWebpMethod
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.webp.method = 4;

  XCTAssertNotEqualObjects(object, object2);
}


- (void)testIsNotEqualOnDifferentWebpImageHint
{
  const auto object = makeDefaultConfiguration();
  const auto object2 = makeDefaultConfiguration();

  object2.webp.imageHint = FSPConfigurationWebpImageHintPicture;

  XCTAssertNotEqualObjects(object, object2);
}

#pragma mark - Helpers

static Configuration makeDefaultInternalConfiguration()
{
  auto configuration = Configuration{};
  configuration.general.defaultBackgroundColor(image::Color{255, 0, 0});
  configuration.general.interpretMetadata(false);
  configuration.general.propagateChromaSamplingModeFromSource(true);
  configuration.general.chromaSamplingModeOverride(Configuration::General::ChromaSamplingModeOverride::S444);
  configuration.jpeg.useTrellis(false);
  configuration.jpeg.useProgressive(true);
  configuration.jpeg.useOptimizeScan(false);
  configuration.jpeg.useCompatibleDcScanOpt(true);
  configuration.jpeg.usePsnrQuantTable(true);
  configuration.png.useInterlacing(true);
  configuration.png.compressionLevel(Configuration::Png::CompressionLevelBestCompression);
  configuration.webp.method(2);
  configuration.webp.imageHint(Configuration::Webp::ImageHint::Photo);
  return configuration;
}

static FSPConfiguration *makeDefaultConfiguration()
{
  const auto generalConfiguration = [[FSPConfigurationGeneral alloc] initWithDefaultBackgroundColor:UIColor.redColor
                                                                                  interpretMetadata:NO
                                                              propagateChromaSamplingModeFromSource:YES
                                                                         chromaSamplingModeOverride:FSPConfigurationChromaSamplingModeOverride444];
  const auto jpegConfiguration = [[FSPConfigurationJpeg alloc] initWithUseTrellis:NO
                                                                   useProgressive:YES
                                                                  useOptimizeScan:NO
                                                        useCompatibleDCScanOption:YES
                                                                usePSNRQuantTable:YES];
  const auto pngConfiguration = [[FSPConfigurationPng alloc] initWithUseInterlacing:YES
                                                                   compressionLevel:FSPPngCompressionLevelBestCompression];
  const auto webpConfiguration = [[FSPConfigurationWebp alloc] initWithMethod:2
                                                                    imageHint:FSPConfigurationWebpImageHintPhoto];
  return [[FSPConfiguration alloc] initWithGeneral:generalConfiguration
                                              jpeg:jpegConfiguration
                                               png:pngConfiguration
                                              webp:webpConfiguration];
}

@end
