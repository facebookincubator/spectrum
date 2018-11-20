// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/GTestTestUtils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace testutils {
namespace test {

// Deactivated as we cannot (easily) catch test failures. Expected output below
// TEST(GTestHelper, whenNothingThrow_thenTestFailure) {
//   std::uint8_t a;
//   ASSERT_SPECTRUM_THROW((void)a, "");
// }
// stdout: ====STANDARD OUT====
// stdout: spectrum/test/unit/helper/GTestHelperTest.cpp:15: Failure
// stdout: Expected: an SpectrumException is thrown.
// stdout:   Actual: no exception is thrown.

// Deactivated as we cannot (easily) catch test failures. Expected output below
// TEST(GTestHelper, whenWrongThrown_thenTestFailure) {
//   ASSERT_SPECTRUM_THROW(throw std::runtime_error(""), "");
// }
// stdout: ====STANDARD OUT====
// stdout: spectrum/test/unit/helper/GTestHelperTest.cpp:20: Failure
// stdout: Expected: an SpectrumException is thrown.
// stdout:   Actual: it throws a different kind.

// Deactivated as we cannot (easily) catch test failures. Expected output below
// TEST(GTestHelper, whenWrongNameThrown_thenTestFailure) {
//   ASSERT_SPECTRUM_THROW(SPECTRUM_ERROR("a"), "");
// }
// stdout: ====STANDARD OUT====
// stdout: spectrum/test/unit/helper/GTestHelperTest.cpp:25: Failure
// stdout:       Expected: spectrumException.name
// stdout:       Which is: "a"
// stdout: To be equal to: ""

TEST(testutils_GTestHelper, whenThrownExceptionMachesExpected_thenTestSuccess) {
  ASSERT_SPECTRUM_THROW(SPECTRUM_ERROR("a"), "a");
}

} // namespace test
} // namespace testutils
} // namespace spectrum
} // namespace facebook
