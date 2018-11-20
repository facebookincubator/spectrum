// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegUtilities.h"

#include <spectrum/plugins/jpeg/LibJpegConstants.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace {

std::vector<core::DataRange> extractDataRangesForMarker(
    jpeg_decompress_struct& libJpegDecompressInfo,
    const std::uint8_t marker) {
  std::vector<core::DataRange> dataRanges;

  for (auto currentMarker = libJpegDecompressInfo.marker_list;
       currentMarker != nullptr;
       currentMarker = currentMarker->next) {
    if (currentMarker->marker == marker) {
      if (currentMarker->data != nullptr && currentMarker->data_length > 0) {
        dataRanges.emplace_back(
            core::DataRange{currentMarker->data, currentMarker->data_length});
      }
    }
  }

  return dataRanges;
}
} // namespace

void saveMetadataMarkers(jpeg_decompress_struct& libJpegDecompressInfo) {
  jpeg_save_markers(
      &libJpegDecompressInfo, static_cast<int>(JPEG_APP1), 0xFFFF);
  jpeg_save_markers(
      &libJpegDecompressInfo, static_cast<int>(JPEG_APP2), 0xFFFF);
}

image::Metadata readMetadata(jpeg_decompress_struct& libJpegDecompressInfo) {
  return image::Metadata{
      extractDataRangesForMarker(libJpegDecompressInfo, JPEG_APP1),
      extractDataRangesForMarker(libJpegDecompressInfo, JPEG_APP2)};
}

void writeMetadata(
    jpeg_compress_struct& libJpegCompressInfo,
    const image::Metadata& metadata) {
  if (metadata.empty()) {
    return;
  }

  const auto entriesData = metadata.entries().makeData();
  if (!entriesData.empty()) {
    jpeg_write_marker(
        &libJpegCompressInfo,
        JPEG_APP1,
        entriesData.data(),
        static_cast<unsigned int>(entriesData.size()));
  }

  const auto colorProfileData = metadata.iccProfile().makeEncodedData();
  for (auto iterator = colorProfileData.cbegin();
       iterator != colorProfileData.cend();
       ++iterator) {
    jpeg_write_marker(
        &libJpegCompressInfo,
        JPEG_APP2,
        iterator->data(),
        static_cast<unsigned int>(iterator->size()));
  }
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
