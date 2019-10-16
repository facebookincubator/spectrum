---
id: supported_image_formats
title: Supported Image Formats
---

The current release of Spectrum supports JPEG, PNG and WebP images. Support for additional image formats can be added via its plugin structure. We think that adding support for [AVIF (AV1 Image Format)](https://aomediacodec.github.io/av1-avif/) would be great!

## JPEG

JPEG files are supported through [mozjpeg](https://github.com/mozilla/mozjpeg/). The mozjpeg encoder provides up to a 20% improvement of encoding rate compared to standard system encoder. This comes at the cost of a higher encoding time.

#### Support
- Spectrum supports lossless transcodes between JPEG files. To leverage it, the following options must be set:
  - Encode requirement mode must be `Any` or `Lossless`. See [encoding images](encoding_images.md) for more info.
  - No crop requirement or its mode must be `ExactOrSmaller`. See [cropping images](cropping_images.md) for more info.
  - No rotate requirement or should only contain a degrees rotation (no flips / or forcing the up orientation). See [rotating images](rotating_images.md) for more info.
- Images can be encoded as either baseline or progressive JPEG files.
- Operations between baseline JPEGs may be more memory efficient perform than to, from or between progressively encoded images. This is because the image can be processed scanline by scanline.
- [Wizards](https://github.com/mozilla/mozjpeg/blob/master/wizard.txt) can control trellis quantization, scan optimization and choose from predefined quantization tables.
- [Chroma sampling mode](https://images.guide/#chroma-subsampling) can be chosen (420 or 444) to improve quality for graphical content.
- EXIF metadata (especially orientation) is supported.

#### Limitations
- Only 8-bit JPEG images are supported as Mozjpeg doesn't allow being built to support both 8-bit and 12-bit images at the same time.

## PNG

PNG files are supported through [libpng](http://www.libpng.org/pub/png/libpng.html).

#### Support
- When encoding, interlaced output can be enabled via the configuration.

#### Limitations
- The compression performance of the underlying zlib isn’t exposed as a user setting yet.

## WEBP

WEBP files are supported through [libwebp](https://github.com/webmproject/libwebp).

#### Support
- When encoding, an “image hint” (e.g. graphical, photo) can be set and might be considered by libwebp.
- Images can either be encoded as lossy or lossless WebP.
