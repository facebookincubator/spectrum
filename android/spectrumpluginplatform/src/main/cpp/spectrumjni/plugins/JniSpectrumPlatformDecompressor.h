// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <spectrum/io/IEncodedImageSource.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/image/Specification.h>
#include <spectrum/codecs/IDecompressor.h>
#include <spectrumjni/JniBaseTypes.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace platform {

/**
* Wrapping class for the SpectrumPlatformDecompressor.
*/
class JSpectrumPlatformDecompressor : public facebook::jni::JavaClass<JSpectrumPlatformDecompressor> {
public:
    static constexpr const char* kJavaDescriptor = "Lcom/facebook/spectrum/plugins/SpectrumPlatformDecompressor;";
    image::Specification getImageSpecification();
    facebook::jni::local_ref<jni::JBitmap> readBitmap();

    static facebook::jni::local_ref<JSpectrumPlatformDecompressor> make(const std::vector<std::uint8_t>& content);
};

/**
 * Providing the IDecompressor implementation using the JSpectrumPlatformDecompressor to be used by the native plugin.
 */
class JniPlatformDecompressor :  public codecs::IDecompressor {
private:
    facebook::jni::local_ref<JSpectrumPlatformDecompressor> _jSpectrumPlatformDecompressor;
    facebook::jni::local_ref<jni::JBitmap> _jBitmap;

    folly::Optional<image::Specification> _imageSpecification;
    std::size_t _outputScanline = 0;

public:
    explicit JniPlatformDecompressor(
            io::IImageSource& source,
            const folly::Optional<image::Ratio>& samplingRatio);

    ~JniPlatformDecompressor() override = default;

    image::Specification sourceImageSpecification() override;
    image::Specification outputImageSpecification() override;
    std::unique_ptr<image::Scanline> readScanline() override;

private:
    void _ensureImageSpecificationRead();
    void _ensureBitmapRead();
};

} // namespace platform
} // namespace plugins
} // namespace spectrum
} // namespace facebook
