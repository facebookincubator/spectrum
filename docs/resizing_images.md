---
id: resizing_images
title: Resizing Images
---

Resizing images is a crucial transformation to reduce the output file size. It is expressed as a requirement for the final image instead of defining the actual operations. This allows Spectrum to determine the most efficient combination of different methods.

## Resizing methods

Spectrum utilizes two methods for changing the resolution of an image:

- _Sampling_: With sampling the decoder will drop pixel information while decoding. This is the most desirable process as it reduces computational costs of decoding and never allocates the full resolution in memory. This can only be done for certain ratios. Example: mozjpeg allows sampling ratios of _i/8_ for _i={1..16}_.

- _Interpolation_: In cases where sampling does not allow for a result that fulfills the request or the decompressor does not provide support, Spectrum uses classical interpolation. By default it uses the [magic kernel](http://www.johncostella.com/magic/). A configuration option allows using bicubic interpolation.

## Resizing parameters

When expressing the size requirement, the provided resize mode defines the desired precision of the final size. Allowing more flexibility of the target resolution usually results in more efficient execution.

- _Exact_: If the input image was larger than the target size, the final image's longest side fit exactly with one of the provided target boundaries. Where no sampling ratio results in the exact size, interpolation is used.

- _Exact or smaller_: If the input image was larger than the target size, it will fit within the provided target boundaries. Spectrum might choose a slightly smaller size if it allows to just use sampling ratios. This usually results in a more efficient execution.

- _Exact or larger_: Similar to the previous case, but instead allows the image to be larger than the target boundaries. This increases the likelihood that the most efficient execution can be used.

> Spectrum will never upscale an image

## Code samples

The following code ensures that the final images do not have any side length larger than 2048. By setting the mode to `ExactOrSmaller` it provides Spectrum more flexibility to choose an efficient execution strategy:

### Android

```java
final ResizeRequirement resizeRequirement = new ResizeRequirement(
    ResizeRequirement.Mode.EXACT_OR_SMALLER,
    new ImageSize(2048, 2048));

// Pass resize requirement to the options
```

The Android API also provides a convenience methods on the options builder for this common operations:

```java
final TranscodeOptions transcodeOptions = TranscodeOptions.Builder(/*...*/)
    .resize(Mode.EXACT_OR_SMALLER, 2048)
    .build();
```

### iOS

```objc
FSPResizeRequirement *resizeRequirement = [FSPResizeRequirement
    initWithMode:FSPResizeRequirementModeExactOrSmaller
      targetSize:CGSizeMake(2048, 2048)];

// Pass resize requirement to the options
```
