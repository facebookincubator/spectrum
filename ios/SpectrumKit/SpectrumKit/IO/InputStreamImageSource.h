// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#include <spectrum/io/IEncodedImageSource.h>

namespace facebook { namespace spectrum { namespace io {

class InputStreamImageSource : public IEncodedImageSource {
public:
  InputStreamImageSource(NSInputStream *inputStream);
  InputStreamImageSource(InputStreamImageSource&&) = default;
  InputStreamImageSource(const InputStreamImageSource &other) = delete;

  ~InputStreamImageSource() override;

  std::size_t read(char* const destination, const size_t length) override;
  std::size_t getTotalBytesRead() const override;
  std::size_t available() override;

private:
  NSInputStream *_inputStream;
  std::size_t _totalBytesRead = 0;
};

}}} // namespace
