// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#include <spectrum/io/IEncodedImageSink.h>

namespace facebook { namespace spectrum { namespace io {

class OutputStreamImageSink : public IEncodedImageSink {
public:
  OutputStreamImageSink(NSOutputStream *outputStream);
  OutputStreamImageSink(const OutputStreamImageSink&) = delete;
  OutputStreamImageSink(OutputStreamImageSink&&) = default;

  ~OutputStreamImageSink() override;

  void setConfiguration(
    const image::Size &imageSize,
    const image::pixel::Specification &pixelSpecification) override;

protected:
  void _write(const char *const source, const size_t length) override;

private:
  NSOutputStream *_outputStream;
};

}}} // namespace
