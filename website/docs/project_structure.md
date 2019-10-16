---
id: project_structure
title: Project Structure
---

The repository’s structure revolves around three main components: the C++ core library and the Android and iOS wrappers. Each of them comes with unit and integration tests plus a sample app per wrapper. The core C++ library contains all the shared code. Subsequently, the Android and iOS wrappers mirror the core's API.

## C++

The C++ library is depended upon by both wrappers.

- `cpp/spectrum/`: Contains the core library implementation including the C++ API, internal models and all necessary code for decision making.
  - `plugins/`: Subfolders contain bundled codecs (e.g. JPEG, PNG & WebP). They are excluded from the main compilation target and form separate build targets.
  - `test/`: GTests for both the code library and bundled plugins.
  - `testutils/`: Additional helpers that are used by the tests.

## Android

The Android wrapper is built in Java and calls the shared C++ code through a JNI layer. The build is setup using Gradle. See the [Contributing on Android](contributing_android.md) section for more details.

- `android/src/main/java`: Contains the public Alchemist API and the calls into the JNI layer.
- `android/src/main/cpp/`: The JNI integration that converts between the Java objects and C++ types.
- `android/src/test/`: Unit tests for the Alchemist API.
- `android/src/androidTest/`: Integration tests that both perform end-to-end tests using SSIM comparison and particular checks of JNI conversions. Not yet open-sourced.
- `android/sample/src/main/`: A sample app that demonstrates how to integrate Alchemist into your project.
- `androidLibs/fbjni`: Internal library in (mostly) C++ that helps to write readable and correct JNI code.
- `androidLibs/third-party`: Destination for all external libraries (e.g. mozjpeg). They are downloaded during build and patched with our configuration.
- `**/CMakeLists.txt`: We use the NDK’s preferred CMake build system. The CMake build files are found both for the JNI layer, external libraries, and the core library (see above) throughout the repository.

## iOS

The iOS wrapper is built in Objective-C++ and calls the C++ library directly. It currently only supports CocoaPods as a dependency manager. See the [Contributing on iOS](contributing_ios.md) section for more details.

- `ios/SpectrumKit/`: The framework's root folder.
  - `SpectrumKit/`: Sources of the main target.
  - `SpectrumKitPlugins/`: Subfolders contain Objective-C bundled codecs.
  - `SpectrumKitTests/`: Wrapper's unit tests.
  - `SpectrumKitInstrumentationTests/`: Wrapper's integration tests.
  - `SpectrumKitInstrumentationTestsHelpers/`: Helpers for the integration tests.
- `ios/SpectrumKitSample/`: SpectrumKit's sample app.
