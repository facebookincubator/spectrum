// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "BaseRecipe.h"

#include <spectrum/Configuration.h>
#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/decisions/BaseDecision.h>
#include <spectrum/core/proc/CroppingScanlineProcessingBlock.h>
#include <spectrum/core/proc/RotationScanlineProcessingBlock.h>
#include <spectrum/core/proc/ScalingScanlineProcessingBlock.h>
#include <spectrum/core/proc/ScanlineConversion.h>
#include <spectrum/core/proc/ScanlinePump.h>

#include <folly/Optional.h>

#include <array>
#include <memory>

namespace facebook {
namespace spectrum {
namespace core {
namespace recipes {

image::Specification BaseRecipe::perform(const Operation& operation) const {
  const auto& parameters = operation.parameters;
  const auto decisions = decisions::BaseDecision::calculate(operation);

  // processing blocks
  std::vector<std::unique_ptr<proc::ScanlineProcessingBlock>> processingBlocks;

  auto decompressor =
      operation.makeDecompressor(decisions.resize.getSamplingRatio());

  const auto scanlineGenerator = [&decompressor] {
    return decompressor->readScanline();
  };

  // (1) cropping
  if (decisions.resize.shouldCrop()) {
    const auto cropRequirement = decisions.resize.cropRequirement();
    const auto croppingInput = decisions.resize.sizeAfterSampling();
    processingBlocks.push_back(
        std::make_unique<proc::CroppingScanlineProcessingBlock>(
            parameters.inputImageSpecification.pixelSpecification,
            croppingInput,
            cropRequirement->apply(croppingInput)));
  }

  // (2) scaling
  if (decisions.resize.shouldScale()) {
    processingBlocks.push_back(
        std::make_unique<proc::ScalingScanlineProcessingBlock>(
            parameters.inputImageSpecification.pixelSpecification,
            decisions.resize.sizeAfterCropping(),
            decisions.resize.sizeAfterScaling(),
            operation.configuration.general.samplingMethod()));
  }

  // (3) rotation
  if (decisions.orientation.shouldRotatePixels()) {
    processingBlocks.push_back(
        std::make_unique<proc::RotationScanlineProcessingBlock>(
            parameters.inputImageSpecification.pixelSpecification,
            decisions.resize.sizeAfterScaling(),
            decisions.orientation.orientation));
  }

  auto compressor =
      operation.makeCompressor(decisions.outputImageSpecification);

  const auto scanlineConverter = proc::makeScanlineConverter(
      parameters.inputImageSpecification.pixelSpecification,
      decisions.outputImageSpecification.pixelSpecification,
      operation.configuration.general.defaultBackgroundColor());

  // scanline consumer
  const auto scanlineConsumer = [scanlineConverter = scanlineConverter.get(),
                                 &compressor = *compressor](auto scanline) {
    compressor.writeScanline(
        scanlineConverter->convertScanline(std::move(scanline)));
  };

  // run chain
  proc::ScanlinePump scanlinePump(
      scanlineGenerator,
      std::move(processingBlocks),
      scanlineConsumer,
      decompressor->outputImageSpecification().size.height);
  scanlinePump.pumpAll();

  return decisions.outputImageSpecification;
}

Rule BaseRecipe::makeRule() {
  return {
      .name = "base",
      .requiresEqualInputOutputFormat = false,
      .isPassthrough = false,
      .cropSupport = Rule::CropSupport::Exact,
      .resizeSupport = Rule::ResizeSupport::Exact,
      .rotateSupport = Rule::RotateSupport::MultipleOf90Flip,
      .recipeFactory = &std::make_unique<BaseRecipe>,
  };
}

} // namespace recipes
} // namespace core
} // namespace spectrum
} // namespace facebook
