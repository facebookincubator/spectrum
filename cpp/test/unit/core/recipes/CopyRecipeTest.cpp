// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/recipes/CopyRecipe.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/core/Constants.h>
#include <spectrum/testutils/TestUtils.h>

#include <memory>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace recipes {
namespace test {

static void assertCopiedCorrectlyForGivenString(const std::string& string) {
  auto source = io::testutils::makeVectorImageSource(string);
  auto sink = io::testutils::FakeImageSink{};
  const auto recipe = CopyRecipe{};
  const auto operation = core::testutils::makeOperationFromIO(source, sink);

  recipe.perform(operation);

  ASSERT_EQ(string, sink.stringContent());
}

TEST(core_recipes_CopyRecipe, whenGivenEmpty_thenSinkEmpty) {
  const std::string string;

  assertCopiedCorrectlyForGivenString(string);
}

TEST(
    core_recipes_CopyRecipe,
    whenGivenStringLessThanBufferSize_thenSinkMatches) {
  const std::string string(core::DefaultBufferSize - 1, 'x');

  assertCopiedCorrectlyForGivenString(string);
}

TEST(
    core_recipes_CopyRecipe,
    whenGivenStringEqualToBufferSize_thenSinkMatches) {
  const std::string string(core::DefaultBufferSize, 'x');

  assertCopiedCorrectlyForGivenString(string);
}

TEST(
    core_recipes_CopyRecipe,
    whenGivenStringLargerThanBufferSizes_thenSinkMatches) {
  const std::string string(core::DefaultBufferSize + 1, 'x');

  assertCopiedCorrectlyForGivenString(string);
}

} // namespace test
} // namespace recipes
} // namespace core
} // namespace spectrum
} // namespace facebook
