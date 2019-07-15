// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/VectorImageSink.h>

#include <string>

namespace facebook {
namespace spectrum {
namespace io {
namespace testutils {

/**
 * An ImageSink that writes to a std::string that can be accessed from tests
 * to assert the written content.
 */
class FakeImageSink : public VectorBitmapImageSink {
 public:
  std::string stringContent();
};

} // namespace testutils
} // namespace io
} // namespace spectrum
} // namespace facebook
