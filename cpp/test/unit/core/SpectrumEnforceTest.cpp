// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/SpectrumEnforce.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace helper {

TEST(SpectrumEnforce, whenConditionTrue_thenNothing) {
  SPECTRUM_ENFORCE_IF_NOT(true);
}

TEST(SpectrumEnforce, whenConditionFalse_thenThrow) {
  ASSERT_THROW(SPECTRUM_ENFORCE_IF_NOT(false), SpectrumException);
}

TEST(SpectrumEnforce, whenUnreachable_thenThrow) {
  ASSERT_THROW(
      SPECTRUM_UNREACHABLE_STRING(error::Unknown, std::string("test")),
      SpectrumException);
}

} // namespace helper
} // namespace spectrum
} // namespace facebook
