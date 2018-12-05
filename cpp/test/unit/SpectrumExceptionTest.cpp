// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/SpectrumException.h>

#include <gtest/gtest.h>

#include <cstring>

namespace facebook {
namespace spectrum {
namespace test {

TEST(SpectrumException, whenBuilding_nothingObservableHappens) {
  const auto name = "name";
  const auto message = std::string{"message"};
  const auto function = "function";
  const auto line = 42;

  const auto exception = SpectrumException{name, message, function, line};

  const auto expectedLocation = "function:42";
  const auto expectedDescription = "name#message-function:42";

  ASSERT_EQ(name, exception.name);
  ASSERT_EQ(message, exception.message);
  ASSERT_EQ(expectedLocation, exception.location);
  ASSERT_EQ(expectedDescription, exception.description);
  ASSERT_EQ(0, std::strcmp(expectedDescription, exception.what()));
}

} // namespace test
} // namespace spectrum
} // namespace facebook
