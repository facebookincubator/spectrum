// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/image/Specification.h>
#include <spectrum/requirements/Rotate.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {

struct OrientationDecision {
  /**
   * Orientation that needs to be applied to the image manually (swapping
   * bytes).
   */
  image::Orientation orientation;

  /**
   * The orientation that needs to be stored in the image's metadata.
   */
  image::Orientation metadataOrientation;

  /**
   * Size of the image as per its bytes representation. Does not take into
   * account metadata orientation.
   */
  image::Size size;

  /**
   * Returns the size of the image taking into account the metadata orientation.
   * It won't necessarily match the size of the image as per its bytes.
   */
  image::Size metadataAwareSize() const;

  /**
   * Returns true if the image bytes needs to be rotated.
   */
  bool shouldRotatePixels() const;

  /**
   * Creates an orientation decision.
   *
   * @param rotateRequirement The rotate requirement of the operation.
   * @param orientation The image orientation.
   * @param size The image size.
   * @param configuration The configuration of the operation.
   * @param compressorSupportsSettingMetadata True if the compressor allows
   * encoding metadata.
   */
  static OrientationDecision calculate(
      const folly::Optional<requirements::Rotate>& rotateRequirement,
      const image::Orientation orientation,
      const image::Size& size,
      const Configuration& configuration,
      const bool compressorSupportsSettingMetadata);

 private:
  OrientationDecision() = default;
};

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
