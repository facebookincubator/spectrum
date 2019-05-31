---
id: faq
title: FAQ
---

If you have identified a common question and answer that should be featured here, please do not hesitate to create an issue or pull request.

### How do I reduce the APK size impact of Spectrum on Android?

Spectrum ships with a considerable amount of native C++ code. By default the Android build system will bundle all four available platform architectures with your app. Have a look at our instructions on [how to configure your `build.gradle` file and to enable ABI split](getting_started_android.md#update-your-project-s-gradle-configuration).

You might also want to look into [just including plugins for the image formats that you want to support](getting_started_android.md#loading-specific-plugins).

### How can I contribute to Spectrum?

Have a look at the [issues on GitHub that are marked with `good first issue`](https://github.com/facebookincubator/spectrum/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22). Those are small to medium sized features that we will support. To get started also look at our contributing pages for [Android](contributing_android.md) and [iOS](contributing_ios.md).

### How fast is Spectrum?

When creating Spectrum, our focus has been on the "photo upload" use-case. This process consists of the preprocessing of the image and its transmission over the network. As the latter is less predictable and often the primary bottleneck, we generally favor doing more computation in the preprocessing step to reduce the payload that will be transmitted. This becomes visible through choices such as using [MozJpeg as the standard JPEG implementation](supported_image_formats.md#jpeg).

Spectrum's design generally focusses on tasks that create encoded image output (such as encoding and transcoding). For specialized operations (e.g. processing Bitmaps), other tools (e.g. using parallel computing on the GPU) are usually more performant.
