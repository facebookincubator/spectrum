// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/DataRange.h>
#include <spectrum/image/Metadata.h>

#include <mozjpeg/jpeglib.h>

#include <vector>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

/**
 * Marks the APP1 & APP2 to be saved in the decompress struct.
 *
 * @param libJpegDecompressInfo The structure to save the markers in.
 */
void saveMetadataMarkers(jpeg_decompress_struct& libJpegDecompressInfo);

/**
 * Reads metadata from the jpeg decompress structure.
 *
 * @param libJpegDecompressInfo The structure to extract the metadata from.
 * @return A newly created Metadata. Will be empty if the corresponding markers
 * haven't been saved.
 */
image::Metadata readMetadata(jpeg_decompress_struct& libJpegDecompressInfo);

/**
 * Writes metadata into the jpeg compress structure.
 *
 * @param libJpegCompressInfo The structure to write the metadata in.
 * @param metadata The metadata to write.
 */
void writeMetadata(
    jpeg_compress_struct& libJpegCompressInfo,
    const image::Metadata& metadata);

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
