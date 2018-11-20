// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPSSIMCalculator.h"

#define FSPImageSimilaritySSIMWindowSize 8
#define FSPImageSimilaritySSIMWindowArea (FSPImageSimilaritySSIMWindowSize * FSPImageSimilaritySSIMWindowSize)

static const NSUInteger FSPImageSimilaritySSIMWindowStep = 4;
static const NSUInteger FSPImageSimilaritySSIMChannelsCount = 4;
static const CGFloat FSPImageSimilaritySSIMK1 = 0.01f;
static const CGFloat FSPImageSimilaritySSIMK2 = 0.03f;
static const CGFloat FSPImageSimilaritySSIMMaxValue = 256.0f;
static const CGFloat FSPImageSimilaritySSIMC1 = FSPImageSimilaritySSIMK1 * FSPImageSimilaritySSIMMaxValue * FSPImageSimilaritySSIMK1 * FSPImageSimilaritySSIMMaxValue;
static const CGFloat FSPImageSimilaritySSIMC2 = FSPImageSimilaritySSIMK2 * FSPImageSimilaritySSIMMaxValue * FSPImageSimilaritySSIMK2 * FSPImageSimilaritySSIMMaxValue;

#pragma mark - SSIM Helpers

static CGFloat FSPComputeAverageForChannels(int const *const channels)
{
  NSUInteger average = 0;
  for (NSUInteger i = 0; i < FSPImageSimilaritySSIMWindowArea; ++i) {
    average += channels[i];
  }

  return (CGFloat)average / (CGFloat)FSPImageSimilaritySSIMWindowArea;
}

static CGFloat FSPComputeVarianceForChannels(int const *const channels, const CGFloat average)
{
  CGFloat variance = 0.0f;
  for (NSUInteger i = 0; i < FSPImageSimilaritySSIMWindowArea; ++i) {
    variance += (channels[i] - average) * (channels[i] - average);
  }

  return variance / (CGFloat)FSPImageSimilaritySSIMWindowArea;
}

static CGFloat FSPComputeCovarianceForChannels(const int *const lhsChannels, const int *const rhsChannels,
                                               const CGFloat lhsAverage, const CGFloat rhsAverage)
{
  CGFloat covariance = 0.0f;
  for (NSUInteger i = 0; i < FSPImageSimilaritySSIMWindowArea; ++i) {
    const CGFloat lhsDelta = lhsChannels[i] - lhsAverage;
    const CGFloat rhsDelta = rhsChannels[i] - rhsAverage;
    covariance += lhsDelta * rhsDelta;
  }

  return covariance / (CGFloat)FSPImageSimilaritySSIMWindowArea;
}

static CGFloat FSPComputeSSIMForChannels(const int *const lhsChannels, const int *const rhsChannels)
{
  const CGFloat lhsAverage = FSPComputeAverageForChannels(lhsChannels);
  const CGFloat lhsVariance = FSPComputeVarianceForChannels(lhsChannels, lhsAverage);
  const CGFloat rhsAverage = FSPComputeAverageForChannels(rhsChannels);
  const CGFloat rhsVariance = FSPComputeVarianceForChannels(rhsChannels, rhsAverage);
  const CGFloat covariance = FSPComputeCovarianceForChannels(lhsChannels, rhsChannels, lhsAverage, rhsAverage);
  const CGFloat t1 = 2 * lhsAverage * rhsAverage + FSPImageSimilaritySSIMC1;
  const CGFloat t2 = 2 * covariance + FSPImageSimilaritySSIMC2;
  const CGFloat t3 = (lhsAverage * lhsAverage) + (rhsAverage * rhsAverage) + FSPImageSimilaritySSIMC1;
  const CGFloat t4 = lhsVariance + rhsVariance + FSPImageSimilaritySSIMC2;
  return (t1 * t2) / (t3 * t4);
}

#pragma mark - FSPImageData

typedef struct
{
  uint8_t const* bytes;
  CGContextRef context;
} FSPImageData;

static FSPImageData FSPMakeImageDataFromImage(CGImageRef image)
{
  // Inspired from https://developer.apple.com/library/content/qa/qa1509/_index.html

  const size_t imageHeight = CGImageGetHeight(image);
  const size_t imageWidth = CGImageGetWidth(image);

  // Declare the number of bytes per row. Each pixel in the bitmap in this
  // example is represented by 4 bytes; 8 bits each of red, green, blue, and
  // alpha.
  const NSInteger bitmapBytesPerRow = imageWidth * FSPImageSimilaritySSIMChannelsCount;

  const CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);

  NSCAssert(colorSpace != NULL, @"Unable to get color space");

  // Allocate memory for image data. This is the destination in memory
  // where any drawing to the bitmap context will be rendered.
  void *bitmapData = malloc(bitmapBytesPerRow * imageHeight);
  if (bitmapData == NULL) {
    CGColorSpaceRelease(colorSpace);
    NSCAssert(false, @"Unable to alloc bitmap data");
  }

  // Create the bitmap context. We want pre-multiplied ARGB, 8-bits
  // per component. Regardless of what the source image format is
  // (CMYK, Grayscale, and so on) it will be converted over to the format
  // specified here by CGBitmapContextCreate.
  CGContextRef context = CGBitmapContextCreate(bitmapData,
                                               imageWidth,
                                               imageHeight,
                                               8, // bits per component
                                               bitmapBytesPerRow,
                                               colorSpace,
                                               kCGImageAlphaPremultipliedFirst);
  CGColorSpaceRelease(colorSpace);

  if (context == NULL) {
    free(bitmapData);
    NSCAssert(false, @"Unable to create context");
  }


  // Draw the image to the bitmap context. Once we draw, the memory
  // allocated for the context for rendering will then contain the
  // raw image data in the specified color space.
  CGContextDrawImage(context, CGRectMake(0, 0, imageWidth, imageHeight), image);

  // Now we can get a pointer to the image data associated with the bitmap
  // context.
  uint8_t const *const bytes = (uint8_t const *const)CGBitmapContextGetData(context);

  NSCAssert(bytes != NULL, @"Unable to get bitmap bytes");

  FSPImageData data;

  data.bytes = bytes;
  data.context = context;

  return data;
}

static void FSPCleanupImageData(const FSPImageData *imageData)
{
  if (imageData != NULL) {
    CGContextRelease(imageData->context);
    free((void *)imageData->bytes);
  }
}

#pragma mark - SSIM

CGFloat FSPComputeSSIMFactorBetween(UIImage *left, UIImage *right)
{
  NSCParameterAssert(left != nil);
  NSCParameterAssert(right != nil);
  NSCParameterAssert(CGSizeEqualToSize(left.size, right.size));
  NSCAssert1(left.size.width >= FSPImageSimilaritySSIMWindowSize, @"Image width must be >= than %d", FSPImageSimilaritySSIMWindowSize);
  NSCAssert1(left.size.height >= FSPImageSimilaritySSIMWindowSize, @"Image height must be >= than %d", FSPImageSimilaritySSIMWindowSize);

  FSPImageData leftImageData = FSPMakeImageDataFromImage(left.CGImage);
  FSPImageData rightImageData = FSPMakeImageDataFromImage(right.CGImage);
  int leftChannels[FSPImageSimilaritySSIMWindowArea];
  int otherChannels[FSPImageSimilaritySSIMWindowArea];

  CGFloat ssimSum = 0;
  NSUInteger ssimCount = 0;

  for (NSUInteger x = 0; x < left.size.width - FSPImageSimilaritySSIMWindowSize; x += FSPImageSimilaritySSIMWindowStep) {
    for (NSUInteger y = 0; y < left.size.height - FSPImageSimilaritySSIMWindowSize; y += FSPImageSimilaritySSIMWindowStep) {

      for (NSUInteger channel = 0; channel < FSPImageSimilaritySSIMChannelsCount; ++channel) {
        for (NSUInteger yy = 0; yy < FSPImageSimilaritySSIMWindowSize; ++yy) {
          for (NSUInteger xx = 0; xx < FSPImageSimilaritySSIMWindowSize; ++xx) {
            const NSUInteger byteIndex = FSPImageSimilaritySSIMChannelsCount * ((y + yy) * left.size.width + (x + xx));
            const NSUInteger i = yy * FSPImageSimilaritySSIMWindowSize + xx;
            leftChannels[i] = leftImageData.bytes[byteIndex + channel];
            otherChannels[i] = rightImageData.bytes[byteIndex + channel];
          }
        }

        ssimSum += FSPComputeSSIMForChannels(leftChannels, otherChannels);
        ++ssimCount;
      }
    }
  }

  FSPCleanupImageData(&leftImageData);
  FSPCleanupImageData(&rightImageData);

  return ssimSum / (CGFloat)ssimCount;
}
