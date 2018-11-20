// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/DataRange.h>
#include <spectrum/image/metadata/Entries.h>
#include <spectrum/image/metadata/ICCProfile.h>

#include <string>
#include <vector>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * Metadata represents information that is stored in images beside their pixel
 * values. The most common example are EXIF metadata and ICC profiles.
 */
class Metadata {
  metadata::Entries _entries;
  metadata::ICCProfile _iccProfile;

 public:
  Metadata() = default;
  explicit Metadata(
      const std::vector<core::DataRange>& entriesDataRanges,
      const std::vector<core::DataRange>& iccProfileDataRanges);
  explicit Metadata(
      metadata::Entries&& entries,
      metadata::ICCProfile&& iccProfile);

  const metadata::Entries& entries() const;
  const metadata::ICCProfile& iccProfile() const;

  metadata::Entries& entries();
  metadata::ICCProfile& iccProfile();

  /**
   * @return true if no entries are contained within this metadata.
   */
  bool empty() const;

  /**
   * Merges the "other" metadata into this object. Entries in the "other"
   * metadata have precedence over entries in this object.
   */
  void merge(const Metadata& other);

  /**
   * String representation of the metadata object including its entries.
   */
  std::string string() const;

  bool operator==(const Metadata& rhs) const;
  bool operator!=(const Metadata& rhs) const;
};

} // namespace image
} // namespace spectrum
} // namespace facebook
