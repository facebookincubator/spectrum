// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "OperationTestUtils.h"

#include <spectrum/testutils/image/SpecificationTestUtils.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace testutils {

Operation::Parameters makeDummyOperationParameters(
    const image::Format& imageFormat,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement) {
  return Operation::Parameters{
      .inputImageSpecification =
          image::testutils::makeDummyImageSpecification(),
      .outputImageFormat = imageFormat,
      .transformations = {},
      .outputPixelSpecificationRequirement =
          outputPixelSpecificationRequirement,
  };
}

Operation makeOperationFromIO(io::IImageSource& source, io::IImageSink& sink) {
  return Operation{
      .io =
          {
              .source = source,
              .sink = sink,
          },
      .codecs = {.decompressorProvider = {.format = image::formats::Bitmap},
                 .compressorProvider = {.format = image::formats::Bitmap}},
      .parameters = makeDummyOperationParameters(),
      .configuration = Configuration(),
  };
}

} // namespace testutils
} // namespace core
} // namespace spectrum
} // namespace facebook
