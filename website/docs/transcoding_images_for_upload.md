---
id: transcoding_images_for_upload
title: Transcoding Images for Upload
---

The processing of images for upload operations is one of the prime use-cases of Spectrum. The goal is to achieve an adequate balance between image quality and file size.

Both are affected by the image resolution (i.e. the number of pixels) and the encoder parameters. The former will have the most significant impact as modern capture devices deliver 12+ megapixel images. The latter can usually default to a value in the range of 70-90.

## Code sample

The following code provides a plausible configuration for processing images for upload. It ensures that the image will not be larger than 2048 on any side. Setting the Mode `ExactOrSmaller` allows Spectrum to just use the decompressors sampling support. The output image is compressed with a JPEG quality level of 80.

### Android

As most image selection code will provide a `URI` for the user image, it first needs to be resolved to an input stream.

```java
try (final InputStream inputStream = contentResolver.openInputStream(uri)) {
  final TranscodeOptions transcodeOptions =
    TranscodeOptions.Builder(new EncodeRequirement(JPEG, 80))
      .resize(Mode.EXACT_OR_SMALLER, new ImageSize(2048, 2048))
      .build();

  final SpectrumResult result = mSpectrum.transcode(
    EncodedImageSource.from(inputStream),
    EncodedImageSink.from("my/output/file/path/upload.jpeg"),
    transcodeOptions,
    "upload_flow_callsite_identifier");
} catch (final IOException e) {
  // e.g. file not found
} catch (final SpectrumException e) {
  // e.g. invalid input image
}
```

### iOS

Below is an example of encoding a `UIImage` into a `NSData`.

```objc
FSPEncodeRequirement *encodeRequirement =
    [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                 mode:FSPEncodeRequirementModeLossy
                                              quality:80];

FSPTransformations *transformations = [FSPTransformations new];
transformations.resizeRequirement =
    [[FSPResizeRequirement alloc] initWithMode:FSPResizeRequirementModeExactOrSmaller
                                    targetSize:CGSizeMake(2048, 2048)];

FSPEncodeOptions *options =
    [FSPEncodeOptions encodeOptionsWithEncodeRequirement:encodeRequirement
                                         transformations:transformations
                                                metadata:nil
                                           configuration:nil
                     outputPixelSpecificationRequirement:nil];

NSError *error;
FSPResultData *result = [FSPSpectrum.sharedInstance encodeImage:image options:options error:&error];
```
