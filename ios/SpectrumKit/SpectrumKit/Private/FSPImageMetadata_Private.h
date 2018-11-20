// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageMetadata.h>
#include <spectrum/image/Metadata.h>
#include <folly/Optional.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPImageMetadata (Private)

- (facebook::spectrum::image::Metadata)makeInternalMetadata;

- (void)addMetadataFromCGImage:(CGImage *)image;

@end

/**
 * Returns a RationalBase<T> representation of number
 *
 * @param number The number to conver.
 * @return The rational representation of the number.
 */
template <typename T>
facebook::spectrum::image::metadata::RationalBase<T> FSPRationalFromValue(NSNumber *const number);

/**
 * Converts an untyped value to a std::vector<T>.
 *
 * @param value the value to convert. Must either be `NSNumber *` or `NSArray<NSNumber *> *`.
 * @return a vector with the mapped values.
 * @note This method will return an empty array if it finds unexpected value types.
 */
template <typename T>
std::vector<T> FSPInternalValuesFromNumberValues(const id value);

/**
 * Returns 3 rationals representing the decimal. This method is to be used to convert degrees or hours into 3 rationals.
 *
 * @param number The number to convert.
 * @return 3 rationals representing the value.
 */
template <typename T>
std::vector<facebook::spectrum::image::metadata::RationalBase<T>> FBInternalRationalsFromDecimalAngleValue(NSNumber *const number);

NS_ASSUME_NONNULL_END
