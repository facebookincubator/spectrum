// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPConfiguration.h>
#import <SpectrumKit/FSPTransformations.h>
#import <SpectrumKit/FSPCropRequirementProviding.h>
#import <SpectrumKit/FSPEncodeRequirement.h>
#import <SpectrumKit/FSPImageSpecification.h>
#import <SpectrumKit/FSPImageMetadata.h>
#import <SpectrumKit/FSPResizeRequirement.h>
#import <SpectrumKit/FSPRotateRequirement.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * The FSPOptions object encapsulates all parameters for an operation. In most
 * cases subclasses (FSPDecodeOptions, FSPTranscodeOptions, FSPEncodeOptions,
 * FSPTransformOptions) should be used instead of this type
 * directly.
 */
NS_SWIFT_NAME(Options)
@interface FSPOptions : NSObject <NSCopying>

+ (instancetype)new NS_UNAVAILABLE;

/**
 * The encode requirement to apply. Dictates the final image format. If none,
 * bitmap is deduced.
 */
@property (nonatomic, strong, nullable, readonly) FSPEncodeRequirement *encodeRequirement;

/**
 * If set, dictates the pixel specification images should be converted to before being
 * passed to the compressor. An exception will be thrown if the pixel specification
 * doesn't fit the image format.
 */
@property (nonatomic, strong, nullable, readonly) FSPImagePixelSpecification *outputImagePixelSpecification;

/**
 * Optional metadata to consider besiders the ones provided by the encoded image.
 */
@property (nonatomic, copy, nullable, readonly) FSPImageMetadata *metadata;

/**
 * Contains optional transformations such as resize, cropping and rotation.
 */
@property (nonatomic, copy, readonly) FSPTransformations *transformations;

/**
 * Non-none options in the configuration object will override the default configuration.
 */
@property (nonatomic, copy, nullable, readonly) FSPConfiguration *configuration;

/**
 * The option's output format. By default Bitmap will be returned unless an
 * encode requirement is set.
 */
@property (nonatomic, strong, readonly) FSPImageFormat *outputImageFormat;

- (instancetype)init NS_UNAVAILABLE;

- (BOOL)isEqualToOptions:(nullable FSPOptions *)object;

@end

NS_SWIFT_NAME(EncodeOptions)
@interface FSPEncodeOptions : FSPOptions

+ (instancetype)encodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement;

+ (instancetype)encodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement
                                   transformations:(nullable FSPTransformations *)transformations
                                          metadata:(nullable FSPImageMetadata *)metadata
                                     configuration:(nullable FSPConfiguration *)configuration
               outputPixelSpecificationRequirement:(nullable FSPImagePixelSpecification *)outputPixelSpecificationRequirement;
@end

NS_SWIFT_NAME(TranscodeOptions)
@interface FSPTranscodeOptions : FSPOptions

+ (instancetype)transcodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement;

+ (instancetype)transcodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement
                                      transformations:(nullable FSPTransformations *)transformations
                                             metadata:(nullable FSPImageMetadata *)metadata
                                        configuration:(nullable FSPConfiguration *)configuration
                  outputPixelSpecificationRequirement:(nullable FSPImagePixelSpecification *)outputPixelSpecificationRequirement;

@end

NS_ASSUME_NONNULL_END
