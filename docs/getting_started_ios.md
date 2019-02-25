---
id: getting_started_ios
title: Getting Started on iOS
---

While our examples on this website focus on Objective-C, we have designed the API with Swift in mind. All public APIs provide bridging headers. See our [sample app for iOS](sample_apps.md) for an example project written in Swift.

## Adding Spectrum to your project

Spectrum can be integrated through [CocoaPods](https://cocoapods.org). To do so, add the following line in your `Podfile`:

```Podfile
pod 'SpectrumKit'
```

Then run `pod install`.

By default, the `SpectrumKit/Default` sub-spec is used which contains all bundled plugins. See [below](#loading-specific-plugins) if you want to only load specific plugins.

> Spectrum currently doesn't support frameworks in Cocoapods. Please make sure your podfile doesn't `use_frameworks!`.

## Using Spectrum

The following code encodes a `UIImage` to a JPEG that is stored at the given `targetUrl`:

```objc
#import <SpectrumKit/SpectrumKit.h>
#import <SpectrumKit/FSPDefaultPlugin.h>

UIImage *image = // ...
NSURL *targetUrl = // ...
NSError *error;

FSPEncodeRequirement *encodeRequirement =
    [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg];
FSPEncodeOptions *options =
    [FSPEncodeOptions encodeOptionsWithEncodeRequirement:encodeRequirement];
FSPResult *result = [FSPSpectrum.sharedInstance encodeImage:image
                                                toFileAtURL:targetUrl
                                                    options:options
                                                      error:&error];

/// [...]
```

## Loading specific plugins

`FSPDefaultPlugin` loads all bundled plugins (i.e `FSPJpegPlugin`, `FSPPngPlugin` & `FSPWebpPlugin`). It also provides a `sharedInstance` singleton for easy access to `FSPSpectrum`. If you want to load plugins more granularly, only include the desired sub-specs in your `Podfile`. The following example only adds the JPEG plugin:

```Podfile
pod 'SpectrumKit/Plugins/Jpeg'
```

Then run `pod install`.

Once installed instantiate an instance with the desired plugins:

```objc
#import <SpectrumKit/SpectrumKit.h>
#import <SpectrumKit/FSPJpegPlugin.h>


FSPSpectrum *spectrum = [[FSPSpectrum alloc] initWithPlugins:@[[FSPJpegPlugin new]]
                                               configuration:nil];
// [...]
```
