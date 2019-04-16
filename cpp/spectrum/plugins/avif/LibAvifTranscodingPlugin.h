// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Plugin.h>
#include <spectrum/image/Format.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

namespace formats {
/**
 * An IVF formatted file with AV1 payload as generated e.g. using libaom.
 */
static constexpr image::EncodedFormat IvfAv1{"ivfav1"};
} // namespace formats

Plugin makeTranscodingPlugin();

} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
