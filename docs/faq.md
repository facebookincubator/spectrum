---
id: faq
title: FAQ
---

If you have identified a common question and answer that should be featured here, please do not hesitate to create an issue or pull-request.

### Why is Spectrum's version number 0.9.0? Is it ready to use?

We decided to launch with version number 0.9.0 to allow us to incorporate early feedback by the community before finalizing the API. This is also because we are using a different [build tool](https://buckbuild.com/) internally. We aim to reach 1.0.0 before the end of year 2018.

### How do I reduce the APK size impact of Spectrum on Android?

Spectrum ships with a considerable amount of native C++ code. By default the Android build system will bundle all four available platform architectures with your app. Have a look at our instructions on [how to configure your `build.gradle` file and to enable ABI split](getting_started_android.md#update-your-project-s-gradle-configuration).

You might also want to look into [just including plugins for the image formats that you want to support](getting_started_android.md#loading-specific-plugins).

### How can I contribute to Spectrum?

Have a look at the [issues on GitHub that are marked with `good first issue`](https://github.com/facebookincubator/spectrum/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22). Those are small to medium sized features that we will support. To get started also look at our contributing pages for [Android](contributing_android.md) and [iOS](contributing_ios.md).
