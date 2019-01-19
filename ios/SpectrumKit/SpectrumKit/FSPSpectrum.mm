// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPSpectrum.h"

#import "FSPImageSpecification_Private.h"
#import "FSPPrivate.h"
#import "FSPImageMetadata_Private.h"
#import "FSPResult_Private.h"
#import "FSPOptions_Private.h"
#import "BitmapImageSource.h"
#import "NSError+Spectrum_Private.h"
#import "InputStreamImageSource.h"
#import "OutputStreamImageSink.h"
#import "FSPPlugin_Private.h"
#import "FSPConfiguration_Private.h"

#include <spectrum/Spectrum.h>

#import "FSPLog.h"

using namespace facebook::spectrum;

@implementation FSPSpectrum
{
  std::unique_ptr<Spectrum> _internalSpectrum;
}

static std::vector<Plugin> convertPlugins(NSArray<id<FSPPlugin>> *plugins)
{
  std::vector<Plugin> result;
  
  for (id<FSPPlugin_Private> plugin in plugins) {
    result.emplace_back([(id<FSPPlugin_Private>)plugin makeInternalPlugin]);
  }
  
  return result;
}

- (instancetype)initWithPlugins:(NSArray<id<FSPPlugin>> *)plugins configuration:(nullable FSPConfiguration *)configuration
{
  if (self = [super init]) {
    auto internalConfiguration = configuration != nil ? [configuration makeInternalConfiguration] : Configuration{};
    _internalSpectrum = std::make_unique<Spectrum>(convertPlugins(plugins), internalConfiguration);
  }
  
  return self;
}

#pragma mark - Encode

- (FSPResultData *)encodeImage:(UIImage *)image
                       options:(FSPEncodeOptions *)options
                         error:(NSError **)error
{
  const auto outputStream = [NSOutputStream outputStreamToMemory];
  auto imageSink = io::OutputStreamImageSink{outputStream};
  const auto result = [self _runEncodeImage:image
                                       sink:imageSink
                                    options:options
                                      error:error];
  return [self.class resultDataWithResult:result outputStream:outputStream];
}

- (FSPResult *)encodeImage:(UIImage *)image
               toFileAtURL:(NSURL *)toUrl
                   options:(FSPEncodeOptions *)options
                     error:(NSError **)error
{
  FSPReportMustFixIfFalse(toUrl.isFileURL, nil);

  const auto outputStream = [NSOutputStream outputStreamWithURL:toUrl append:NO];
  auto imageSink = io::OutputStreamImageSink{outputStream};
  return [self _runEncodeImage:image sink:imageSink options:options error:error];
}

#pragma mark - Transcode

- (FSPResult *)transcodeImageFromFileAtURL:(NSURL *)fromUrl
                               toFileAtURL:(NSURL *)toUrl
                                   options:(FSPTranscodeOptions *)options
                                     error:(NSError **)error
{
  FSPReportMustFixIfFalse(fromUrl.isFileURL, nil);
  FSPReportMustFixIfFalse(toUrl.isFileURL, nil);

  const auto inputStream = [NSInputStream inputStreamWithURL:fromUrl];
  const auto outputStream = [NSOutputStream outputStreamWithURL:toUrl append:NO];
  return [self _runTranscodeImageFromStream:inputStream
                                   toStream:outputStream
                                    options:options
                                      error:error];
}

- (FSPResultData *)transcodeImageFromFileAtURL:(NSURL *)fromUrl
                                       options:(FSPTranscodeOptions *)options
                                         error:(NSError **)error
{
  FSPReportMustFixIfFalse(fromUrl.isFileURL, nil);

  const auto inputStream = [NSInputStream inputStreamWithURL:fromUrl];
  const auto outputStream = [NSOutputStream outputStreamToMemory];
  const auto result = [self _runTranscodeImageFromStream:inputStream
                                                toStream:outputStream
                                                 options:options
                                                   error:error];
  return [self.class resultDataWithResult:result outputStream:outputStream];
}

- (FSPResult *)transcodeImageFromData:(NSData *)data
                          toFileAtURL:(NSURL *)toUrl
                              options:(FSPTranscodeOptions *)options
                                error:(NSError **)error
{
  FSPReportMustFixIfFalse(data.length > 0, nil);
  FSPReportMustFixIfFalse(toUrl.isFileURL, nil);

  const auto inputStream = [NSInputStream inputStreamWithData:data];
  const auto outputStream = [NSOutputStream outputStreamWithURL:toUrl append:NO];
  return [self _runTranscodeImageFromStream:inputStream
                                   toStream:outputStream
                                    options:options
                                      error:error];
}

- (FSPResultData *)transcodeImageFromData:(NSData *)data
                                  options:(FSPTranscodeOptions *)options
                                    error:(NSError **)error
{
  FSPReportMustFixIfFalse(data.length > 0, nil);

  const auto inputStream = [NSInputStream inputStreamWithData:data];
  const auto outputStream = [NSOutputStream outputStreamToMemory];
  const auto result = [self _runTranscodeImageFromStream:inputStream
                                                toStream:outputStream
                                                 options:options
                                                   error:error];
  return [self.class resultDataWithResult:result outputStream:outputStream];
}

#pragma mark - Private

- (FSPResult *)_runEncodeImage:(UIImage *)image
                          sink:(io::IEncodedImageSink &)sink
                       options:(FSPEncodeOptions *)options
                         error:(NSError **)error
{
  FSPReportMustFixIfNil(image, nil);
  FSPReportMustFixIfNil(options, nil);

  return [self.class _runWithBlock:^Result() {
    auto source = io::BitmapImageSource{image};
    const auto internalOptions = [options makeInternalEncodeOptions];
    return self->_internalSpectrum->encode(source, sink, internalOptions);
  } error:error];
}

- (FSPResult *)_runTranscodeImageFromStream:(NSInputStream *)inputStream
                                   toStream:(NSOutputStream *)outputStream
                                    options:(FSPTranscodeOptions *)options
                                      error:(NSError **)error
{
  FSPReportMustFixIfNil(options, nil);

  return [self.class _runWithBlock:^Result() {
    auto source = io::InputStreamImageSource{inputStream};
    auto sink = io::OutputStreamImageSink{outputStream};
    const auto internalOptions = [options makeInternalTranscodeOptions];
    return self->_internalSpectrum->transcode(source, sink, internalOptions);
  } error:error];
}

+ (FSPResult *)_runWithBlock:(Result (^)())block error:(NSError **)error
{
  try {
    const auto internalResult = block();
    const auto result = [[FSPResult alloc] initWithInternalTranscodeResult:internalResult];

    if (result.didSucceed == NO) {
      [NSError fsp_buildNoRuleFoundError:error];
    }

    return result;
  } catch (const SpectrumException &e) {
    [NSError fsp_buildError:error spectrumException:e];
    return [FSPResult resultForFailure];
  } catch (const std::exception &e) {
    [NSError fsp_buildError:error exception:e];
    return [FSPResult resultForFailure];
  }
}

+ (FSPResultData *)resultDataWithResult:(FSPResult *)result outputStream:(NSOutputStream *)outputStream
{
  NSData *const outputData = [outputStream propertyForKey:NSStreamDataWrittenToMemoryStreamKey];
  return [[FSPResultData alloc] initWithResult:result data:outputData];
}

@end
