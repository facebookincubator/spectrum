// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace testutils {

#define ASSERT_SPECTRUM_THROW(statement, expectedExceptionName)                                \
  do {                                                                                         \
    try {                                                                                      \
      statement;                                                                               \
      GTEST_FATAL_FAILURE_(                                                                    \
          "Expected: an SpectrumException is thrown.\n  Actual: no exception is thrown.");     \
    } catch (const SpectrumException& spectrumException) {                                     \
      ASSERT_EQ(spectrumException.name, expectedExceptionName);                                \
    } catch (...) {                                                                            \
      GTEST_FATAL_FAILURE_(                                                                    \
          "Expected: an SpectrumException is thrown.\n  Actual: it throws a different kind."); \
    }                                                                                          \
  } while (false)

template <size_t N>
void assertArrayContent(
    const std::string& expected,
    const std::array<uint8_t, N>& array) {
  ASSERT_EQ(N, expected.size());
  const auto actual =
      std::string(reinterpret_cast<const char*>(array.data()), N);
  ASSERT_EQ(expected, actual);
}

} // namespace testutils
} // namespace spectrum
} // namespace facebook
