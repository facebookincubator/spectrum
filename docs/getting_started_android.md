---
id: getting_started_android
title: Getting Started on Android
---

To integrate Spectrum into your project, simply add new dependencies to your app module, initialize them at the application level, and construct a new instance of the `Spectrum` class. Then you are all set and ready for executing image operations.

While our examples on this website focus on Java, we have designed the API with Kotlin in mind. Our [sample app for Android](sample_apps.md) is written in Kotlin.

## Update your project's Gradle configuration

Add the default Spectrum dependency that includes all image format plugins:

```groovy
dependencies {
  // Spectrum with support for JPEG, PNG and WebP
  implementation 'com.facebook.spectrum:spectrum-default:1.1.0+'
}
```

As Spectrum bundles with native C++ libraries, it's important to enable ABI split in your build configuration. This keeps the APK size in check:

```groovy
// within your build.gradle in the 'android' extension
splits {
  abi {
    enable true
  }
}
```

## Initialize Spectrum

Spectrum uses the [SoLoader library](https://github.com/facebook/SoLoader) to load native code which needs to be initialized first. Your `Application` class is a good place for this.

```java
public void onCreate() {
  super.onCreate();
  SpectrumSoLoader.init(this);
  ...
}
```

Then you can instantiate Spectrum. The constructor requires a `SpectrumLogger` and a list of `SpectrumPlugins`. After it is constructed the `Spectrum` object can handle multiple operations independently as it is thread-safe. Here, we load the default plugin list that includes support for JPEG, PNG, and WebP:

```java
public void onCreate() {
  ...
  mSpectrum = Spectrum.make(
    new SpectrumLogcatLogger(Log.INFO),
    DefaultPlugins.get()); // JPEG, PNG and WebP plugins
}
```

## Using Spectrum

Now you are ready to use Spectrum in your application. Each of the operations will require a source to read the input from, a sink where the output is created, the corresponding options and a caller context.

```java
mSpectrum.transcode(
  EncodedImageSource.from(inputFile),
  EncodedImageSink.from(outputStream),
  TranscodeOptions.Builder(new EncodeRequirement(JPEG, 80)).build(),
  "my_callsite_identifier");
```

The other sections show how to [transcode images for upload](transcoding_images_for_upload) and how to execute other common operations. When reading or writing to user storage, do not forget to [request the correct permissions](https://developer.android.com/training/data-storage/files#ExternalStoragePermissions).

## Loading specific plugins

If you later decide that you only want to bundle with certain plugins, you can select more granular dependencies:

```groovy
dependencies {
  implementation 'com.facebook.spectrum:spectrum-core:1.1.0+'
  implementation 'com.facebook.spectrum:spectrum-jpeg:1.1.0+'
  implementation 'com.facebook.spectrum:spectrum-png:1.1.0+'
  implementation 'com.facebook.spectrum:spectrum-webp:1.1.0+'
}
```
