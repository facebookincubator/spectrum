// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPConfiguration.h>
#import <SpectrumKit/FSPResult.h>
#import <SpectrumKit/FSPOptions.h>
#import <SpectrumKit/FSPPlugin.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Main entry point for Spectrum. Once created, a Spectrum instance is thread
 * safe. While creating a new instance with plugins is a lightweight operation,
 * it is recommended to keep a Spectrum instance around if possible.
 */
NS_SWIFT_NAME(Spectrum)
@interface FSPSpectrum: NSObject

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithPlugins:(NSArray<id<FSPPlugin>> *)plugins configuration:(nullable FSPConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

/**
 * Encodes the image into a data object.
 *
 * @param image The image to encode.
 * @param options The options describing the operation.
 * @param error A pointer to an error object. Can be nil.
 * @return An object containing information about the executed operation and the encoded data.
 */
- (FSPResultData *)encodeImage:(UIImage *)image
                       options:(FSPEncodeOptions *)options
                         error:(NSError *_Nullable *)error NS_SWIFT_NAME(encodeImage(_:options:error:));

/**
 * Encodes the image into a file on disk.
 *
 * @param image The image to encode.
 * @param toUrl The location to write the encoded image at. Must be a file url.
 * @param options The options describing the operation.
 * @param error A pointer to an error object. Can be nil.
 * @return An object containing information about the executed operation.
 */
- (FSPResult *)encodeImage:(UIImage *)image
               toFileAtURL:(NSURL *)toUrl
                   options:(FSPEncodeOptions *)options
                     error:(NSError *_Nullable *)error NS_SWIFT_NAME(encodeImage(_:toFileAt:options:error:));

/**
 * Transcodes an image on disk to a location on disk.
 *
 * @param fromUrl The location to read the encoded image from. Must be a file url.
 * @param toUrl The location to write the encoded image at. Must be a file url.
 * @param options The options describing the operation.
 * @param error A pointer to an error object. Can be nil.
 * @return An object containing information about the executed operation.
 */
- (FSPResult *)transcodeImageFromFileAtURL:(NSURL *)fromUrl
                               toFileAtURL:(NSURL *)toUrl
                                   options:(FSPTranscodeOptions *)options
                                     error:(NSError *_Nullable *)error NS_SWIFT_NAME(transcodeImage(fromFileAt:toFileAt:options:error:));

/**
 * Transcodes an image on disk into a data object.
 *
 * @param fromUrl The location to read the encoded image from. Must be a file url.
 * @param options The options describing the operation.
 * @param error A pointer to an error object. Can be nil.
 * @return An object containing information about the executed operation and the encoded data.
 */
- (FSPResultData *)transcodeImageFromFileAtURL:(NSURL *)fromUrl
                                       options:(FSPTranscodeOptions *)options
                                         error:(NSError *_Nullable *)error NS_SWIFT_NAME(transcodeImage(fromFileAt:options:error:));

/**
 * Transcodes an image from a data object to a location on disk.
 *
 * @param data The data containing the encoded image.
 * @param toUrl The location to write the encoded image at. Must be a file url.
 * @param options The options describing the operation.
 * @param error A pointer to an error object. Can be nil.
 * @return An object containing information about the executed operation.
 */
- (FSPResult *)transcodeImageFromData:(NSData *)data
                          toFileAtURL:(NSURL *)toUrl
                              options:(FSPTranscodeOptions *)options
                                error:(NSError *_Nullable *)error NS_SWIFT_NAME(transcodeImage(from:toFileAt:options:error:));

/**
 * Transcodes an image from a data object into a data object.
 *
 * @param data The data containing the encoded image.
 * @param options The options describing the operation.
 * @param error A pointer to an error object. Can be nil.
 * @return An object containing information about the executed operation and the encoded data.
 */
- (FSPResultData *)transcodeImageFromData:(NSData *)data
                                  options:(FSPTranscodeOptions *)options
                                    error:(NSError *_Nullable *)error NS_SWIFT_NAME(transcodeImage(from:options:error:));

@end

NS_ASSUME_NONNULL_END
