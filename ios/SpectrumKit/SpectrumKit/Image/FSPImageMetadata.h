// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(ImageMetadata)
@interface FSPImageMetadata : NSObject <NSCopying>

+ (instancetype)imageMetadataFromImage:(UIImage *)image;
+ (instancetype)imageMetadataWithDictionary:(nullable NSDictionary<NSString *, id> *)dictionary;

@property (nonatomic, copy, readonly) NSDictionary<NSString *, id> *dictionary;

@end

NS_ASSUME_NONNULL_END
