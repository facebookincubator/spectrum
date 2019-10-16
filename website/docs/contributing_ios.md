---
id: contributing_ios
title: Contributing on iOS
---

## Prerequisites

Before checking out the code please ensure that both Xcode and Cocoapods and installed and up-to-date.

## Preparing the project

[Fork the Spectrum project](https://guides.github.com/activities/forking/) or checkout the sources of our main repository:

```
git clone https://github.com/facebookincubator/spectrum.git
```

## Run the sample app

The easiest way to build and test changes is through the sample app. To do so:

* Navigate to `spectrum/ios/SpectrumKitSample`.
* Run `pod install` to get the sample app configured with the development pod.
* open `SpectrumKitSample.xcworkspace`.

## Testing your changes

Tests are currently part of the open source release but are not yet configured in Cocoapods.
