// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegLosslessRotateAndCropRecipe.h"

#include <spectrum/plugins/jpeg/LibJpegDctTransformer.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

image::Specification LibJpegLosslessRotateAndCropRecipe::perform(
    const core::Operation& operation) const {
  LibJpegDctTransformer dctTransformer{operation.io.source, operation.io.sink};

  dctTransformer.setRotateRequirement(
      operation.parameters.transformations.rotateRequirement);
  dctTransformer.setCropRequirement(
      operation.parameters.transformations.cropRequirement);

  // apply execute
  dctTransformer.applyAndFinish();

  // output type matches input type
  return image::Specification{
      .size = dctTransformer.getOutputSize(),
      .format = image::formats::Jpeg,
      .pixelSpecification =
          operation.parameters.inputImageSpecification.pixelSpecification,
      .chromaSamplingMode =
          operation.parameters.inputImageSpecification.chromaSamplingMode,
  };
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
