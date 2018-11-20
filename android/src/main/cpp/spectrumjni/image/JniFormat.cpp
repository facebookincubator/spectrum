// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniFormat.h"

#include <spectrum/Options.h>

#include <fbjni/fbjni.h>
#include <folly/FixedString.h>

namespace facebook {
namespace spectrum {
namespace image {

image::Format JFormat::toNative() const {
  const auto jIdentifier = identifier();
  const auto fixedIdentifier =
      folly::FixedString<10>{jIdentifier.c_str(), jIdentifier.size()};

  if (encoded()) {
    return image::EncodedFormat(fixedIdentifier);
  } else {
    return image::Format(fixedIdentifier);
  }
}

facebook::jni::local_ref<JFormat> JFormat::fromNative(
    const image::Format& format) {
  const auto identifier = facebook::jni::make_jstring(format.identifier());
  if (format.encoded()) {
    return JEncodedFormat::fromNative(format);
  } else {
    return JFormat::newInstance(identifier);
  }
}

std::string JFormat::identifier() const {
  static const auto field = javaClassStatic()->getField<jstring>("identifier");
  return getFieldValue(field)->toStdString();
}

bool JFormat::encoded() const {
  static const auto method =
      javaClassStatic()->getMethod<jboolean()>("isEncoded");
  return method(self());
}

image::EncodedFormat JEncodedFormat::toNative() const {
  const auto jIdentifier = identifier();
  const auto fixedIdentifier =
      folly::FixedString<10>{jIdentifier.c_str(), jIdentifier.size()};

  return image::EncodedFormat(fixedIdentifier);
}

facebook::jni::local_ref<JEncodedFormat> JEncodedFormat::fromNative(
    const image::Format& format) {
  const auto identifier = facebook::jni::make_jstring(format.identifier());
  return newInstance(identifier);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
