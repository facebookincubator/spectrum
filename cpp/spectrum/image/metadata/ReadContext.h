// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstddef>
#include <cstdint>

#include <spectrum/core/SpectrumEnforce.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {

/**
 * Context passed through for parsing.
 */
struct ReadContext {
  const std::uint8_t* const tiffHeaderBegin;
  const std::uint8_t* const dataEnd;
  const bool isLittleEndianEncoded;

  ReadContext(
      const std::uint8_t* const dataBegin,
      const std::size_t dataLength,
      const std::uint8_t* const tiffHeaderBegin,
      const bool isLittleEndianEncoded)
      : tiffHeaderBegin(tiffHeaderBegin),
        dataEnd(dataBegin + dataLength),
        isLittleEndianEncoded(isLittleEndianEncoded) {
    SPECTRUM_ENFORCE_IF_NOT(dataBegin != nullptr);
    SPECTRUM_ENFORCE_IF_NOT(tiffHeaderBegin != nullptr);
    SPECTRUM_ENFORCE_IF_NOT(
        tiffHeaderBegin > dataBegin && tiffHeaderBegin < dataEnd);
  }
};

} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
