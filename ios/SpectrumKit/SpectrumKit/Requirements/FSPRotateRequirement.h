// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Represents the rotation logic that needs to be applied to the operation.
 */
NS_SWIFT_NAME(RotateRequirement)
@interface FSPRotateRequirement : NSObject

/** A rotate requirement without any values does not make sense */
+ (instancetype)new NS_UNAVAILABLE;

/**
 * Creates a new rotate requirement.
 */
+ (instancetype)rotateRequirementWithDegrees:(NSInteger)rotationDegrees;

/**
 * Creates a new rotate requirement.
 */
+ (instancetype)rotateRequirementWithForceUpOrientation:(BOOL)forceUpOrientation;

/**
 * Creates a new rotate requirement.
 */
+ (instancetype)rotateRequirementWithDegrees:(NSInteger)rotationDegrees
                      shouldFlipHorizontally:(BOOL)shouldFlipHorizontally
                        shouldFlipVertically:(BOOL)shouldFlipVertically
                    shouldForceUpOrientation:(BOOL)shouldForceUpOrientation;

/**
 * The angle to apply.
 */
@property (nonatomic, assign, readonly) NSInteger degrees;

/**
 * Wether the image should be flipped horizontally.
 */
@property (nonatomic, assign, readonly) BOOL shouldFlipHorizontally;

/**
 * Wether the image should be flipped vertically.
 */
@property (nonatomic, assign, readonly) BOOL shouldFlipVertically;

/**
 * Prevents the orientation to be encoded in the metadata. Instead the bytes of the image will be straighten to match the up orientation.
 */
@property (nonatomic, assign, readonly) BOOL shouldForceUpOrientation;

/** A rotate requirement without any values does not make sense */
- (instancetype)init NS_UNAVAILABLE;

/**
 * Compares two rotate requirements.
 *
 * @param object The object rotate requirement to compare it to.
 */
- (BOOL)isEqualToRotateRequirement:(nullable FSPRotateRequirement *)object;

@end

NS_ASSUME_NONNULL_END
