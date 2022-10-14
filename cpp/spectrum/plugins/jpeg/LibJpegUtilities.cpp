// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegUtilities.h"

#include <spectrum/plugins/jpeg/LibJpegConstants.h>
#include <string>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace {

constexpr int XMP_MAX_SIZE_IN_JPG = 65536;

std::string extractXmp(jpeg_decompress_struct& libJpegDecompressInfo) {
  for (auto currentMarker = libJpegDecompressInfo.marker_list;
       currentMarker != nullptr;
       currentMarker = currentMarker->next) {
    if (currentMarker->data != nullptr && currentMarker->data_length > 29 &&
        currentMarker->data_length < XMP_MAX_SIZE_IN_JPG) {
      // XMP string has 29 bytes signature of null terminated
      // ascii string (http://ns.adobe.com/xap/1.0/)
      auto signature = std::string((char*)currentMarker->data, 29);
      if (0 == strcmp("http://ns.adobe.com/xap/1.0/", signature.c_str())) {
        auto xmp =
            std::string((char*)currentMarker->data, currentMarker->data_length);
        return xmp;
      }
    }
  }
  return std::string("");
}

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
      extractDataRangesForMarker(libJpegDecompressInfo, JPEG_APP2),
      extractXmp(libJpegDecompressInfo)};
}

void writeMetadata(
    jpeg_compress_struct& libJpegCompressInfo,
    const image::Metadata& metadata) {
  if (metadata.empty()) {
    return;
  }

  const auto entriesData = metadata.entries().makeData();
  if (!metadata.xmp().empty()) {
    jpeg_write_marker(
        &libJpegCompressInfo,
        JPEG_APP1,
        (unsigned char*)metadata.xmp().data(),
        static_cast<unsigned int>(metadata.xmp().size()));
  }
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
