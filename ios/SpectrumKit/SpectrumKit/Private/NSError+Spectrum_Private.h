// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import "FSPLog.h"

#include <spectrum/core/SpectrumEnforce.h>

NS_ASSUME_NONNULL_BEGIN

FSP_LINK_REQUIRE(NSError_Spectrum_Private)
@interface NSError (Spectrum_Private)

+ (void)fsp_buildError:(NSError *_Nullable *)error
             exception:(const std::exception &)exception;

+ (void)fsp_buildError:(NSError *_Nullable *)error
     spectrumException:(const facebook::spectrum::SpectrumException &)exception;

+ (void)fsp_buildError:(NSError **)error name:(nullable NSString *)category;

+ (void)fsp_buildError:(NSError **)error
                  name:(nullable NSString *)name
               message:(nullable NSString *)message
              location:(nullable NSString *)location;

+ (void)fsp_buildNoRuleFoundError:(NSError **)error;

@end

std::string FSPMakeMessageFromError(NSError *error);

NS_ASSUME_NONNULL_END
