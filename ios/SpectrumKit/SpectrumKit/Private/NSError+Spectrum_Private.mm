// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "NSError+Spectrum_Private.h"

#include <string>

#include <spectrum/Spectrum.h>
#include <folly/Range.h>

#import "NSError+Spectrum.h"

using namespace facebook::spectrum;

FSP_LINKABLE(NSError_Spectrum_Private)
@implementation NSError (Spectrum_Private)

static NSDictionary<NSString *, id> *FSPBuildUserInfoDictionaryFromValues(NSString *const name, NSString *const message, NSString *const location)
{
  NSMutableDictionary<NSString *, id> *const userInfo = [NSMutableDictionary new];

  if (name.length > 0) {
    userInfo[FSPErrorNameKey] = name;
  }

  if (message.length > 0) {
    userInfo[FSPErrorMessageKey] = message;
  }

  if (location.length > 0) {
    userInfo[FSPErrorLocationKey] = location;
  }

  return userInfo;
}

+ (void)fsp_buildError:(NSError **)error
     spectrumException:(const SpectrumException &)exception
{
  [self fsp_buildError:error
                  name:!exception.name.empty() ? [NSString stringWithUTF8String:exception.name.c_str()] : nil
               message:exception.message.hasValue() && !exception.message->empty() ? [NSString stringWithUTF8String:exception.message->c_str()] : nil
              location:!exception.location.empty() ? [NSString stringWithUTF8String:exception.location.c_str()] : nil];
}

+ (void)fsp_buildError:(NSError **)error
             exception:(const std::exception &)exception
{
  [self fsp_buildError:error
                  name:exception.what() != nullptr ? [NSString stringWithUTF8String:exception.what()] : nil
               message:nil
              location:nil];
}

+ (void)fsp_buildError:(NSError **)error
                  name:(NSString *)message
{
  [self fsp_buildError:error
                  name:message
               message:nil
              location:nil];
}

+ (void)fsp_buildNoRuleFoundError:(NSError **)error
{
  [NSError fsp_buildError:error
                     name:[NSString stringWithUTF8String:core::error::NoMatchingRule.start()]];
}

+ (void)fsp_buildError:(NSError **)error
                  name:(NSString *)name
               message:(NSString *)message
              location:(NSString *)location
{
  if (error == nil) {
    return;
  }

  *error = [NSError errorWithDomain:FSPSpectrumErrorDomain
                               code:255
                           userInfo:FSPBuildUserInfoDictionaryFromValues(name, message, location)];
}

std::string FSPMakeMessageFromError(NSError *error)
{
  NSString *const message = error.localizedDescription ?: error.localizedFailureReason;

  return std::string(message != nil ? message.UTF8String : "unknonw");
}

@end
