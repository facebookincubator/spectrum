// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IBitmapImageSource.h>
#include <spectrumjni/JniBaseTypes.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace io {

class JniBitmapImageSource : public io::IBitmapImageSource {
 private:
  facebook::jni::alias_ref<jni::JBitmap> _jInputBitmap;
  image::Specification _imageSpecification;

  std::size_t _offsetBytes = 0;

 public:
  explicit JniBitmapImageSource(
      facebook::jni::alias_ref<jni::JBitmap> jInputBitmap);

  JniBitmapImageSource(const JniBitmapImageSource&) = delete;
  JniBitmapImageSource(JniBitmapImageSource&&) = default;

  ~JniBitmapImageSource() override = default;

  std::size_t read(char* const destination, const std::size_t length) override;

  image::Specification imageSpecification() const override;

  std::size_t getTotalBytesRead() const override;

  std::size_t available() override;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
