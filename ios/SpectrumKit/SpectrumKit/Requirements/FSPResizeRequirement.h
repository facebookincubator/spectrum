// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * The resize requirement mode. Defines the strictness of the resize requirement
 * in accepting values other than the specified target size. In any cases, the
 * resizing operation won't affect the aspect ratio of the image. Resizing will opt
 * to choose for a non-exact resolution if allowed as an optimisation.
 */
typedef NS_ENUM(NSUInteger, FSPResizeRequirementMode) {
  FSPResizeRequirementModeExact,
  FSPResizeRequirementModeExactOrSmaller,
  FSPResizeRequirementModeExactOrLarger,
} NS_SWIFT_NAME(ResizeRequirementMode);

/**
 * Represents a resize requirement. Contains both a mode to specify
 * the resizing rules and a target size.
 */
NS_SWIFT_NAME(ResizeRequirement)
@interface FSPResizeRequirement : NSObject

+ (instancetype)new NS_UNAVAILABLE;

/**
 * Creates an exact resize requirement
 *
 * @param targetSize The size to resize to.
 * @return The newly created object.
 */
+ (instancetype)resizeRequirementExactWithTargetSize:(CGSize)targetSize;

/**
 * The mode of the resize requirement.
 */
@property (nonatomic, readonly, assign) FSPResizeRequirementMode mode;

/**
 * The target size of the resize requirement. If set to `CGSizeZero` the requirement will be ignored.
 */
@property (nonatomic, readonly, assign) CGSize targetSize;

- (instancetype)init NS_UNAVAILABLE;

/**
 * Creates a resize requirement.
 *
 * @param mode The resizing mode.
 * @param targetSize The size to resize to.
 * @return The newly created object.
 */
- (instancetype)initWithMode:(FSPResizeRequirementMode)mode
                  targetSize:(CGSize)targetSize NS_DESIGNATED_INITIALIZER;

/**
 * Compares two resize requirements.
 *
 * @param object The object to compare it to.
 * @return YES if both objects are equal.
 */
- (BOOL)isEqualToResizeRequirement:(nullable FSPResizeRequirement *)object;

@end

NS_ASSUME_NONNULL_END
