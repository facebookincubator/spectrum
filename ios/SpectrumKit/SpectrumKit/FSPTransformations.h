// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPCropRequirementProviding.h>
#import <SpectrumKit/FSPResizeRequirement.h>
#import <SpectrumKit/FSPRotateRequirement.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Image transformations that can be applied to an operation.
 */
NS_SWIFT_NAME(Transformations)
@interface FSPTransformations : NSObject <NSCopying>

/**
 * The rotate requirement to apply.
 */
@property (nonatomic, strong, nullable) FSPRotateRequirement *rotateRequirement;

/**
 * The resize requirement to apply.
 */
@property (nonatomic, strong, nullable) FSPResizeRequirement *resizeRequirement;

/**
 * The crop requirement to apply.
 */
@property (nonatomic, strong, nullable) NSObject<FSPCropRequirementProviding> *cropRequirement;

- (instancetype)initWithResizeRequirement:(nullable FSPResizeRequirement *)resizeRequirement
                        rotateRequirement:(nullable FSPRotateRequirement *)rotateRequirement
                          cropRequirement:(nullable NSObject<FSPCropRequirementProviding> *)cropRequirement;

- (BOOL)isEqualToTransformations:(nullable FSPTransformations *)object;

@end

NS_ASSUME_NONNULL_END
