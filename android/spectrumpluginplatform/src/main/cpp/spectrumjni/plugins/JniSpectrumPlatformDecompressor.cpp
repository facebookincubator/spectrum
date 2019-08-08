// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumPlatformDecompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrumjni/image/JniSpecification.h>
#include <spectrumjni/BitmapPixelsLock.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace platform {

//
// JSpectrumPlatformDecompressor
//

image::Specification JSpectrumPlatformDecompressor::getImageSpecification() {
    static const auto method =
            javaClassStatic()->getMethod<facebook::jni::local_ref<image::JSpecification>()>(
                    "getImageSpecification");
    return method(self())->toNative();
}

facebook::jni::local_ref<jni::JBitmap> JSpectrumPlatformDecompressor::readBitmap() {
    static const auto method =
            javaClassStatic()->getMethod<facebook::jni::local_ref<jni::JBitmap>()>(
                    "readBitmap");
    return method(self());
}

facebook::jni::local_ref<JSpectrumPlatformDecompressor>
JSpectrumPlatformDecompressor::make(const std::vector<std::uint8_t> &content) {
    auto jByteArray = facebook::jni::JArrayByte::newArray(content.size());

    // copy read bytes to java heap from source: for this we reinterpret the content vector as
    // signed chars (Java's byte type) which have the same underlying byte size
    static_assert(
            sizeof(signed char) == sizeof(std::uint8_t),
            "wtf signed char and std::uint8_t differ in size");
    jByteArray->setRegion(0, content.size(), reinterpret_cast<const signed char *>(content.data()));

    return JSpectrumPlatformDecompressor::newInstance(jByteArray);
}

//
// JniPlatformDecompressor
//

JniPlatformDecompressor::JniPlatformDecompressor(
        io::IImageSource &source,
        const folly::Optional <image::Ratio> &samplingRatio) {
    std::vector<std::uint8_t> content{};
    content.reserve(source.available());

    std::vector<std::uint8_t> buffer(core::DefaultBufferSize);
    std::size_t bytesRead;
    while ((bytesRead = source.read(reinterpret_cast<char *const>(buffer.data()), buffer.size())) > 0) {
        content.insert(content.end(), buffer.begin(), buffer.begin() + bytesRead);
    }

    _jSpectrumPlatformDecompressor = JSpectrumPlatformDecompressor::make(content);
}

image::Specification JniPlatformDecompressor::sourceImageSpecification() {
    _ensureImageSpecificationRead();
    return *_imageSpecification;
}

image::Specification JniPlatformDecompressor::outputImageSpecification() {
    return sourceImageSpecification();
}

std::unique_ptr<image::Scanline> JniPlatformDecompressor::readScanline() {
    _ensureImageSpecificationRead();
    _ensureBitmapRead();
    if (_outputScanline >= _imageSpecification->size.height) {
        return nullptr;
    }

    jni::BitmapPixelsLock bmpLock(
            facebook::jni::Environment::current(), _jBitmap.get());
    const uint8_t* pixelPtr = bmpLock.getPixelsPtr();

    SPECTRUM_ERROR_CSTR_IF(
            pixelPtr == nullptr,
            codecs::error::DecompressorFailure,
            "failed_to_lock_bitmap");

    const auto& pixelSpec = _imageSpecification->pixelSpecification;
    const auto width = _imageSpecification->size.width;
    auto scanline = std::make_unique<image::Scanline>(pixelSpec, width);

    SPECTRUM_ERROR_CSTR_IF(
            scanline->sizeBytes() != bmpLock.getScanlineSizeBytes(),
            codecs::error::DecompressorFailure,
            "unexpected_input_scanline_size");

    // copy data from bitmap to scanline
    const std::size_t offset = _outputScanline * scanline->sizeBytes();
    memcpy(scanline->data(), pixelPtr + offset, scanline->sizeBytes());
    ++_outputScanline;

    return scanline;
}

void JniPlatformDecompressor::_ensureBitmapRead() {
    if (!_jBitmap) {
        _jBitmap = _jSpectrumPlatformDecompressor->readBitmap();
    }
}

void JniPlatformDecompressor::_ensureImageSpecificationRead() {
    if (!_imageSpecification.has_value()) {
        _imageSpecification = _jSpectrumPlatformDecompressor->getImageSpecification();
    }
}

} // namespace platform
} // namespace plugins
} // namespace spectrum
} // namespace facebook
