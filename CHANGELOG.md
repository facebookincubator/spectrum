# 1.3.0 (Jan, 2022)
Please see GitHub for detailed changes: https://github.com/facebookincubator/spectrum/compare/v1.2.0...v1.3.0

# 1.2.0 (Jun 5, 2020)
- Error is now reported for images with a pixel format kCGImagePixelFormatRGB555 (iOS)
- Fixed an issue by which metadata wouldn't be added to the encoded image
- Avoids copying thumbnail metadata on transcoded images
- Renamed VectorImageSink to VectorBitmapImageSink
- Fixed Kotlin compatibility issues from SoLoaderWrapper
- Fixed an issue that would result in ignoring the output pixel specification in certain situations
- Increased test coverage
- Fixed heap-use-after-free in avif decompressor
- Support RGBA for PNG compression
- Fixed compilation on Windows
- Fixed memory leak in ScalingScanlineProcessingBlock
- Updated deps: libavif 0.6.2, libwebp 1.1.0


# 1.1.0 (May 24, 2019)

 - Spectrum now supports modular headers when using cocoapods
 - Spectrum now supports setting the compression level for libpng
 - Spectrum now supports setting a custom SoLoader implementation
 - Fixed Spectrum's PNG encode operations from ARGB input
 - Spectrum's dependency on libwebp is updated to 1.0.2
 - Spectrum has experimental support for decoding AV1 frames in IVF containers
 - Improved documentation including highlights of Kotlin/Swift interoperability
 - Various fixes and improvements

# 1.0.0 (January 17, 2019)

 - Spectrum can now write interlaced PNG files
 - Spectrum now supports consuming JPEG images with 4:1:1 and 4:4:0 chroma subsampling (see GitHub issue 8)
 - Spectrum will default to 4:2:0 chroma subsampling for iOS bitmaps
 - Spectrum builds with proper SIMD support for mozjpeg on Android ARMv7
 - Fixed Spectrum build on case-sensitive file systems
 - Improved documentation
 - Various fixes and improvements

# 0.9.0 (November 20, 2018)

 - Initial release
