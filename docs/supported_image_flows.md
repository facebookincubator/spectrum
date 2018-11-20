---
id: supported_image_flows
title: Supported Image Flows
---

Spectrum's API provides four main methods which cater all possible permutations of having bitmaps or encoded images as either input or output. All of them allow to apply common transformations.

## Operations

Spectrum supports four main types of operations:

- _Encoding_: The operation of converting a bitmap into an encoded image. This operation requires providing an encode requirement to specify the target image's encoding parameters such as format, quality & mode.
- _Transcoding_: The operation of converting an encoded image into another encoded image. Just as encoding, this operation requires providing an encode requirement to specify the target image's encoding parameters such as format, quality & mode. Some image formats, such as JPEG, support lossless transcoding given certain options. For more info, check the [lossless JPEG bullet point](supported_image_formats.md#support).
- _Decoding_: The operation of converting an encoded image into a bitmap.
- _Transforming_: The operation of converting a bitmap into another bitmap.

## Transformations

Each operation supports transformations such as [cropping](cropping_images.md), [resizing](resizing_images.md) & [rotating](rotating_images.md). Applying transformations in conjunction with the decoding / transcoding steps may allow Spectrum to choose a more memory-efficient execution. For example, when uncompressing an image, Spectrum may be able to leverage sampling to avoid ever reading the entire image into memory.
