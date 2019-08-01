---
id: faq
title: FAQ
---

If you have identified a common question and answer that should be featured here, please do not hesitate to create an issue or pull request.

### How do I reduce the APK size impact of Spectrum on Android?

Spectrum ships with a considerable amount of native C++ code. By default the Android build system will bundle all four available platform architectures with your app. Have a look at our instructions on [how to configure your `build.gradle` file and to enable ABI split](getting_started_android.md#update-your-project-s-gradle-configuration).

You might also want to look into [just including plugins for the image formats that you want to support](getting_started_android.md#loading-specific-plugins).

### In which order are the transformations (resize, rotate, and crop) executed?

Interestingly, the actual order of execution does not matter. This is a nice property that allows Spectrum to reoder operations into a more efficient sequence. With regard to the logical order, the following statements make it more clear:

- The crop requirement always logically works on the input image coordinates. I.e. one can imagine that it is executed first.
- The resize requirement gives a guarantee about the output image dimensions. I.e. one can imagine that it is executed last.
- The rotation requirement can be seen as being "logically executed" in between as it does not directly relate to the input or output dimensions.

When actually executing a request, Spectrum might rotate first or resize first. However, it will always ensure that it updates the other transformations such that the logical model still holds. E.g. if Spectrum resizes first, the crop requirement will also be "resized".

### How can I contribute to Spectrum?

Have a look at the [issues on GitHub that are marked with `good first issue`](https://github.com/facebookincubator/spectrum/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22). Those are small to medium sized features that we will support. To get started also look at our contributing pages for [Android](contributing_android.md) and [iOS](contributing_ios.md).

### How fast is Spectrum?

When creating Spectrum, our focus has been on the "photo upload" use-case. This process consists of the preprocessing of the image and its transmission over the network. As the latter is less predictable and often the primary bottleneck, we generally favor doing more computation in the preprocessing step to reduce the payload that will be transmitted. This becomes visible through choices such as using [MozJpeg as the standard JPEG implementation](supported_image_formats.md#jpeg).

Spectrum's design generally focusses on tasks that create encoded image output (such as encoding and transcoding). For specialized operations (e.g. processing Bitmaps), other tools (e.g. using parallel computing on the GPU) are usually more performant.
