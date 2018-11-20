// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace testutils {
namespace test {

//
// Recipe & Recipe::Factory
//

TEST(
    testutils_BaseTestHelper,
    whenCopyingRecipeTranscode_thenSourceCopiedToSink) {
  auto source = io::testutils::makeVectorImageSource("abc");
  auto sink = io::testutils::FakeImageSink{};

  auto options = DecodeOptions();
  const auto recipe = makeLambdaFactoryForCopyRecipe()();
  const auto operation = core::testutils::makeOperationFromIO(source, sink);

  recipe->perform(operation);

  ASSERT_EQ("abc", sink.stringContent());
}

TEST(
    testutils_BaseTestHelper,
    whenConstantOutputRecipeTranscode_thenSinkWithConstantValue) {
  auto source = io::testutils::makeVectorImageSource("abc");
  auto sink = io::testutils::FakeImageSink{};

  const Options options = makeDummyTranscodeOptions();
  const auto recipe = makeLambdaFactoryForConstantOutputRecipe("xyz")();
  const auto operation = core::testutils::makeOperationFromIO(source, sink);

  recipe->perform(operation);

  ASSERT_EQ("xyz", sink.stringContent());
}

TEST(testutils_BaseTestHelper, whenNullRecipeTranscode_thenSinkEmpty) {
  auto source = io::testutils::makeVectorImageSource("abc");
  auto sink = io::testutils::FakeImageSink{};

  const Options options = makeDummyTranscodeOptions();
  const auto recipe = makeLambdaFactoryForNullRecipe()();
  const auto operation = core::testutils::makeOperationFromIO(source, sink);

  recipe->perform(operation);

  ASSERT_EQ("", sink.stringContent());
}

} // namespace test
} // namespace testutils
} // namespace spectrum
} // namespace facebook
