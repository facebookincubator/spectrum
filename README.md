<p align="center">
  <a href="https://libspectrum.io/">
    <img src="website/static/img/spectrum_logo_color.svg" width="20%" />
  </a>
</p>
<h1 align="center">
  Spectrum
</h1>
<p align="center">
  <a href="https://circleci.com/gh/facebookincubator/spectrum">
    <img src="https://circleci.com/gh/facebookincubator/spectrum.svg?style=svg" alt="CircleCI" />
  </a>
  <a href="https://twitter.com/libspectrum">
   <img src="https://img.shields.io/twitter/follow/libspectrum?style=social" alt="Follow @libspectrum" />
  </a>
  <a href="https://github.com/facebookincubator/spectrum/blob/master/LICENSE">
    <img alt="GitHub" src="https://img.shields.io/github/license/facebookincubator/spectrum">
  </a>
  <a href="https://github.com/facebookincubator/spectrum/blob/master/CONTRIBUTING.md">
    <img alt="GitHub" src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg">
  </a>
   <a href="https://github.com/facebookincubator/spectrum/graphs/contributors">
    <img alt="GitHub contributors" src="https://img.shields.io/github/contributors/facebookincubator/spectrum">
  </a>
</p>

Spectrum is a cross-platform image transcoding library that can easily be integrated into an Android or iOS project to efficiently perform common image operations.

The Spectrum APIs incorporate declarative traits. This simplifies the usage for the developer by focusing on the desired outcome. At the same time it enables Spectrum to transparently choose the best execution sequence. Additionally, Spectrum makes common image operations simple by surfacing many difficult operations that are not typically exposed by platform frameworks. For example, some JPEG-to-JPEG operations like cropping are executed lossless, and disabling chroma sub-sampling when saving graphical JPEGs will lead to better quality final products.

Find out more at our [website](https://libspectrum.io).

## Requirements

Spectrum was built with mobile iOS and Android in mind but could be wrapped for any platform.

## Getting started

Please refer to our _Getting Started chapters_ in our documentation to start using Spectrum to transcode your images on [Android](https://libspectrum.io/docs/getting_started_android) and [iOS](https://libspectrum.io/docs/getting_started_ios).

## Join the Spectrum community

Please use our [issues page](https://github.com/facebookincubator/spectrum/issues) to let us know of any problems.

For pull requests, please see the [CONTRIBUTING](./CONTRIBUTING.md) file for information on how to help out.

## License

Spectrum is [MIT-licensed](./LICENSE).
