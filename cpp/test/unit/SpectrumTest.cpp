// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/Spectrum.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <folly/FixedString.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace test {

//
// Test error cases
//

TEST(Spectrum, transcode_whenInputSourceEmpty_thenThrowEmptyInputSource) {
  const auto spectrum = Spectrum();
  auto source = io::testutils::makeVectorImageSource("");
  auto sink = io::testutils::FakeImageSink{};

  ASSERT_SPECTRUM_THROW(
      spectrum.decode(source, sink), spectrum::error::EmptyInputSource);
}

} // namespace test
} // namespace spectrum
} // namespace facebook
