// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniInputStreamImageSourceTest.h"

#include <algorithm>
#include <array>
#include <exception>
#include <memory>
#include <vector>

#include <spectrumjni/io/JniInputStreamImageSource.h>

namespace facebook {
namespace spectrum {
namespace jni {

facebook::jni::local_ref<JniInputStreamImageSourceTest::jhybriddata>
JniInputStreamImageSourceTest::initHybrid(
    facebook::jni::alias_ref<jhybridobject>) {
  return makeCxxInstance();
}

facebook::jni::local_ref<facebook::jni::JArrayByte>
JniInputStreamImageSourceTest::readFromInputStream(
    facebook::jni::alias_ref<io::JInputStream> jInputStream) {
  // wrap as image source
  auto source = std::make_shared<io::JniInputStreamImageSource>(jInputStream);

  std::vector<char> result;

  std::array<char, 256> buffer;
  std::size_t numReadBytes = 0;
  do {
    numReadBytes = source->read(buffer.data(), buffer.size());
    result.insert(result.end(), buffer.data(), buffer.data() + numReadBytes);
  } while (numReadBytes);

  // move content into Java byte array and pass it back
  auto arr = facebook::jni::JArrayByte::newArray(result.size());
  arr->setRegion(
      0, result.size(), reinterpret_cast<const signed char*>(result.data()));
  return arr;
}

jint JniInputStreamImageSourceTest::getAvailableFromInputStream(
    facebook::jni::alias_ref<io::JInputStream> jInputStream) {
  auto source = std::make_shared<io::JniInputStreamImageSource>(jInputStream);
  return source->available();
}

void JniInputStreamImageSourceTest::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", JniInputStreamImageSourceTest::initHybrid),
      makeNativeMethod(
          "readFromInputStream",
          JniInputStreamImageSourceTest::readFromInputStream),
      makeNativeMethod(
          "getAvailableFromInputStream",
          JniInputStreamImageSourceTest::getAvailableFromInputStream),
  });
}

} // namespace jni
} // namespace spectrum
} // namespace facebook
