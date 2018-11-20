// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPOptions.h"
#import "FSPOptions_Private.h"

#import "FSPConfiguration_Private.h"
#import "FSPTransformations_Private.h"
#import "FSPEncodeRequirement_Private.h"
#import "FSPImagePixelSpecification_Private.h"
#import "FSPImageSpecification_Private.h"
#import "FSPPrivate.h"
#import "FSPImageMetadata_Private.h"

#include <spectrum/Options.h>

#import "FSPLog.h"

using namespace facebook::spectrum;

@implementation FSPOptions

#pragma mark - Initialisers

- (instancetype)initWithEncodeRequirement:(nullable FSPEncodeRequirement *)encodeRequirement
                          transformations:(nullable FSPTransformations *)transformations
            outputImagePixelSpecification:(nullable FSPImagePixelSpecification *)outputImagePixelSpecification
                                 metadata:(nullable FSPImageMetadata *)metadata
                            configuration:(nullable FSPConfiguration *)configuration
{
  if (self = [super init]) {
    _encodeRequirement = encodeRequirement;
    _transformations = [transformations copy] ?: [FSPTransformations new];
    _outputImagePixelSpecification = outputImagePixelSpecification;
    _metadata = [metadata copy];
    _configuration = [configuration copy];
  }

  return self;
}

#pragma mark - Properties

- (FSPImageFormat *)outputImageFormat
{
  return _encodeRequirement.format ?: FSPImageFormat.bitmap;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithEncodeRequirement:_encodeRequirement
                                                      transformations:_transformations
                                        outputImagePixelSpecification:_outputImagePixelSpecification
                                                             metadata:_metadata
                                                        configuration:_configuration];
}

#pragma mark - Equality

- (BOOL)isEqualToOptions:(FSPOptions *)object
{
  if (object == nil) {
    return NO;
  }

  return (FSPObjectEqualObject(_encodeRequirement, object.encodeRequirement) &&
          FSPObjectEqualObject(_transformations, object.transformations) &&
          FSPObjectEqualObject(_outputImagePixelSpecification, object.outputImagePixelSpecification) &&
          FSPObjectEqualObject(_metadata, object.metadata) &&
          FSPObjectEqualObject(_configuration, object.configuration));
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPOptions class]]) {
    return [self isEqualToOptions:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return (_encodeRequirement.hash ^ _transformations.hash ^
          _outputImagePixelSpecification.hash ^ _metadata.hash ^ _configuration.hash);
}

#pragma mark - Internal

- (folly::Optional<image::Metadata>)makeInternalMetadata
{
  if (_metadata == nil) {
    return folly::none;
  } else {
    return [_metadata makeInternalMetadata];
  }
}

- (Configuration)makeInternalConfiguration
{
  if (_configuration == nil) {
    return Configuration();
  } else {
    return [_configuration makeInternalConfiguration];
  }
}

- (folly::Optional<image::pixel::Specification>)makeInternalOutputPixelImageSpecification
{
  if (_outputImagePixelSpecification == nil) {
    return folly::none;
  } else {
    return [_outputImagePixelSpecification makeInternalSpecification];
  }
}

@end

@implementation FSPEncodeOptions

+ (instancetype)encodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement
{
  return [[self alloc] initWithEncodeRequirement:encodeRequirement
                                 transformations:nil
                   outputImagePixelSpecification:nil
                                        metadata:nil
                                   configuration:nil];
}

+ (instancetype)encodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement
                                   transformations:(nullable FSPTransformations *)transformations
                                          metadata:(nullable FSPImageMetadata *)metadata
                                     configuration:(nullable FSPConfiguration *)configuration
               outputPixelSpecificationRequirement:(nullable FSPImagePixelSpecification *)outputPixelSpecificationRequirement
{
  FSPReportMustFixIfNil(encodeRequirement, nil);

  return [[self alloc] initWithEncodeRequirement:encodeRequirement
                                 transformations:transformations
                   outputImagePixelSpecification:outputPixelSpecificationRequirement
                                        metadata:metadata
                                   configuration:configuration];
}

- (EncodeOptions)makeInternalEncodeOptions
{
  FSPReportMustFixIfNil(self.encodeRequirement, nil);
  FSPReportMustFixIfNil(self.transformations, nil);

  return EncodeOptions([self.encodeRequirement makeInternalEncodeRequirement],
                       [self.transformations makeInternalTransformations],
                       [self makeInternalMetadata],
                       [self makeInternalConfiguration],
                       [self makeInternalOutputPixelImageSpecification]);
}

@end

@implementation FSPTranscodeOptions

+ (instancetype)transcodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement
{
  return [self transcodeOptionsWithEncodeRequirement:encodeRequirement
                                     transformations:nil
                                            metadata:nil
                                       configuration:nil
                 outputPixelSpecificationRequirement:nil];
}

+ (instancetype)transcodeOptionsWithEncodeRequirement:(FSPEncodeRequirement *)encodeRequirement
                                      transformations:(nullable FSPTransformations *)transformations
                                             metadata:(nullable FSPImageMetadata *)metadata
                                        configuration:(nullable FSPConfiguration *)configuration
                  outputPixelSpecificationRequirement:(nullable FSPImagePixelSpecification *)outputPixelSpecificationRequirement
{
  FSPReportMustFixIfNil(encodeRequirement, nil);

  return [[self alloc] initWithEncodeRequirement:encodeRequirement
                                 transformations:transformations
                   outputImagePixelSpecification:outputPixelSpecificationRequirement
                                        metadata:metadata
                                   configuration:configuration];
}

- (TranscodeOptions)makeInternalTranscodeOptions
{
  FSPReportMustFixIfNil(self.encodeRequirement, nil);
  FSPReportMustFixIfNil(self.transformations, nil);

  return TranscodeOptions([self.encodeRequirement makeInternalEncodeRequirement],
                          [self.transformations makeInternalTransformations],
                          [self makeInternalMetadata],
                          [self makeInternalConfiguration],
                          [self makeInternalOutputPixelImageSpecification]);
}

@end
