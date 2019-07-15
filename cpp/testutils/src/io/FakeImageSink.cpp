// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "FakeImageSink.h"

namespace facebook {
namespace spectrum {
namespace io {
namespace testutils {

std::string FakeImageSink::stringContent() {
  const auto& content = VectorBitmapImageSink::getVectorReference();
  return std::string(content.begin(), content.end());
}

} // namespace testutils
} // namespace io
} // namespace spectrum
} // namespace facebook
