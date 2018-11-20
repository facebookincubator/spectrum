---
id: encoding_images
title: Encoding Images
---

An encode requirement must be set when the target image is compressed. An encode requirement has a few parameters:

- _Format_: Defines the target format the image should be encoded to. E.g `JPEG`.
- _Mode_: Defines how the image should be encoded. Can be `Any`, `Lossy` or `Lossless`.
- _Quality_: The compression quality that should be used. The value must be between 0 and 100. 0 is deemed as unset in which case a default value for the format will be used.

> While JPEG can only perform lossy compression in most cases it may be able to perform lossless transcoding. For more info, check the [lossless JPEG bullet point](supported_image_formats.md#support).

## Code Sample

The following code encodes an image to JPEG with quality 42:

### Android

```java
EncodeRequirement encodeRequirement =
    new EncodeRequirement(
        /* format */ EncodedImageFormat.JPEG,
        /* quality */ 42,
        /* mode */ EncodeRequirement.Mode.LOSSY);

// Pass encode requirement to the options
```

### iOS

```objc
FSPEncodeRequirement *encodeRequirement =
    [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                 mode:FSPEncodeRequirementModeLossy
                                              quality:42];
// Pass encode requirement to the options
```
