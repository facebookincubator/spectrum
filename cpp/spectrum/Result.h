// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Specification.h>

#include <string>

namespace facebook {
namespace spectrum {

/**
 * A Result encapsulates information about the operation (e.g. the
 * determined image format and size) as well as information about the applied
 * rule.
 */
struct Result {
  /**
   * Name of the transcode rule which has performed the operation.
   */
  std::string ruleName;

  /**
   * Detected input image specification for the operation.
   */
  image::Specification inputImageSpecification;

  /**
   * Determined output image specification for the operation.
   */
  image::Specification outputImageSpecification;

  /**
   * Total number of bytes that has been read from the underlying image source
   */
  std::size_t totalBytesRead{0};

  /**
   * Total number of bytes that has been written to the image sink
   */
  std::size_t totalBytesWritten{0};

  /**
   * Time taken in milliseconds to transcode
   */
  std::uint32_t duration;
};

} // namespace spectrum
} // namespace facebook
