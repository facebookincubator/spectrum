// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPResult.h"
#import "FSPResult_Private.h"

#import "FSPImageSpecification_Private.h"

using namespace facebook::spectrum;

@implementation FSPResult

+ (instancetype)resultForFailure
{
  return [[self alloc] initWithDidSucceed:NO
                                 ruleName:nil
                 outputImageSpecification:nil
                           totalBytesRead:0
                        totalBytesWritten:0
                                 duration:0];
}

- (instancetype)initWithDidSucceed:(BOOL)didSucceed
                          ruleName:(nullable NSString *)ruleName
          outputImageSpecification:(nullable FSPImageSpecification *)outputImageSpecification
                    totalBytesRead:(NSUInteger)totalBytesRead
                 totalBytesWritten:(NSUInteger)totalBytesWritten
                          duration:(NSUInteger)duration
{

  if (self = [super init]) {
    _didSucceed = didSucceed;
    _ruleName = [ruleName copy];
    _outputImageSpecification = outputImageSpecification;
    _totalBytesRead = totalBytesRead;
    _totalBytesWritten = totalBytesWritten;
    _duration = duration;
  }

  return self;
}

#pragma mark - Internal

- (instancetype)initWithInternalTranscodeResult:(const Result &)result
{
  return [self initWithDidSucceed:YES
                         ruleName:[NSString stringWithUTF8String:result.ruleName.c_str()]
         outputImageSpecification:[FSPImageSpecification imageSpecificationFromInternalImageSpecification:result.outputImageSpecification]
                   totalBytesRead:static_cast<NSUInteger>(result.totalBytesRead)
                totalBytesWritten:static_cast<NSUInteger>(result.totalBytesWritten)
                         duration:static_cast<NSUInteger>(result.duration)];
}

@end

@implementation FSPResultData

- (instancetype)initWithResult:(FSPResult *)result data:(nullable NSData *)data
{
  if (self = [super init]) {
    _result = result;
    _data = result.didSucceed ? [data copy] : nil;
  }

  return self;
}

+ (instancetype)resultForFailure
{
  return [[self alloc] initWithResult:[FSPResult resultForFailure] data:nil];
}

@end
