// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IEncodedImageSink.h>

#include <cstddef>
#include <fstream>
#include <string>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * An encoded image sink that writes into a file at the specific path.
 */
class FileImageSink : public IEncodedImageSink {
 private:
  std::ofstream ofs;

 public:
  /**
   * Creates a file image sink for the given path. If no such file exists, it is
   * created.
   *
   * @throws SpectrumException if the output stream cannot be initialized
   * (e.g. insufficient permissions, ...).
   */
  explicit FileImageSink(const std::string& path);
  FileImageSink(const FileImageSink&) = delete;
  FileImageSink(FileImageSink&&) = default;

  ~FileImageSink() override = default;

  void setConfiguration(
      const image::Size& imageSize,
      const image::pixel::Specification& pixelSpecification) override;

 protected:
  void _write(const char* const source, const std::size_t length) override;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
