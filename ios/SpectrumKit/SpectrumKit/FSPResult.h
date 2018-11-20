// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageSpecification.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Encapsulates information about the operation (e.g. the
 * determined image format and size) as well as information about the applied
 * rule.
 */
NS_SWIFT_NAME(Result)
@interface FSPResult : NSObject

+ (instancetype)new NS_UNAVAILABLE;
+ (instancetype)resultForFailure;

@property (nonatomic, assign, readonly) BOOL didSucceed;

/**
 * Name of the transcode rule which has performed the operation.
 */
@property (nonatomic, copy, nullable, readonly) NSString *ruleName;

/**
 * Determined output image specification for the operation.
 */
@property (nonatomic, strong, nullable, readonly) FSPImageSpecification *outputImageSpecification;

/**
 * Total number of bytes that have been read.
 */
@property (nonatomic, assign, readonly) NSUInteger totalBytesRead;

/**
 * Total number of bytes that have been written.
 */
@property (nonatomic, assign, readonly) NSUInteger totalBytesWritten;

/**
 * Transcode duration in milliseconds.
 */
@property (nonatomic, assign, readonly) NSUInteger duration;

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithDidSucceed:(BOOL)didSucceed
                          ruleName:(nullable NSString *)ruleName
          outputImageSpecification:(nullable FSPImageSpecification *)outputImageSpecification
                    totalBytesRead:(NSUInteger)totalBytesRead
                 totalBytesWritten:(NSUInteger)totalBytesWritten
                          duration:(NSUInteger)duration NS_DESIGNATED_INITIALIZER;

@end

/**
 * Object containing both a result and a data object. This is the returned object from
 * encode/transcode calls that return an in-memory encoded image.
 */
NS_SWIFT_NAME(ResultData)
@interface FSPResultData : NSObject

+ (instancetype)new NS_UNAVAILABLE;

@property (nonatomic, strong, readonly) FSPResult *result;
@property (nonatomic, copy, nullable, readonly) NSData *data;
@property (nonatomic, copy, readonly, class) FSPResultData *resultForFailure;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithResult:(FSPResult *)result data:(nullable NSData *)data NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
