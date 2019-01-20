// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(ConfigurationJpeg)
@interface FSPConfigurationJpeg : NSObject <NSCopying>

@property (nonatomic, assign) BOOL useTrellis;
@property (nonatomic, assign) BOOL useProgressive;
@property (nonatomic, assign) BOOL useOptimizeScan;
@property (nonatomic, assign) BOOL useCompatibleDCScanOption;
@property (nonatomic, assign) BOOL usePSNRQuantTable;

- (instancetype)initWithUseTrellis:(BOOL)useTrellis
                    useProgressive:(BOOL)useProgressive
                   useOptimizeScan:(BOOL)useOptimizeScan
         useCompatibleDCScanOption:(BOOL)useCompatibleDCScanOption
                 usePSNRQuantTable:(BOOL)usePSNRQuantTable;

- (BOOL)isEqualToConfigurationJpeg:(FSPConfigurationJpeg *)object;

@end

NS_ASSUME_NONNULL_END
