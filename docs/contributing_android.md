---
id: contributing_android
title: Contributing on Android
---

Thank you for your interest in contributing to Spectrum! We try to make the experience as smooth as possible despite Spectrum's numerous external dependencies.

## Prerequisites

Before checking out the code please ensure that the following tools are correctly setup and up-to-date.

- Ensure that you have the latest [Android Studio and SDK tools](https://developer.android.com/studio/) installed on your machine.
- Setup the latest [Native Development Kit (NDK)](https://developer.android.com/ndk/guides/) and verify your setup with [the sample C++ project in Android Studio](https://developer.android.com/studio/projects/add-native-code#new-project).

## Preparing the project

[Fork the Spectrum project](https://guides.github.com/activities/forking/) or checkout the sources of our main repository:

```bash
git clone https://github.com/facebookincubator/spectrum.git
```

The external native dependencies require preparation to correctly show up in Android Studio. Therefore, run the following command to before opening the project in Android Studio:

```bash
cd your/checkout/of/spectrum/
./gradlew prepareNativeLibrary
```

## Testing your changes

We will only accept pull requests that come with a plausible test plan. If your change affects the output of an image operation, provide the input image, the output image before the change, and the output image after the change. Also include any output from `adb logcat`.

Also verify that Android unit tests pass. You can run them using the following command:

```bash
./gradlew test
```

For running the extensive GTests on the native code, please see the [Contributing on iOS section](contributing_ios.md) of this documentation. For native changes we recommend following that guide and to use Xcode.
