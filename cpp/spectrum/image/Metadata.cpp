// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Metadata.h"

namespace facebook {
namespace spectrum {
namespace image {

Metadata::Metadata(
    const std::vector<core::DataRange>& entriesDataRanges,
    const std::vector<core::DataRange>& iccProfileDataRanges,
    const std::string& xmp)
    : _entries{entriesDataRanges},
      _iccProfile{iccProfileDataRanges},
      _xmp(xmp) {}

Metadata::Metadata(
    metadata::Entries&& entries,
    metadata::ICCProfile&& iccProfile,
    std::string&& xmp)
    : _entries(std::move(entries)),
      _iccProfile(std::move(iccProfile)),
      _xmp(std::move(xmp)) {}

bool Metadata::empty() const {
  return _entries.empty() && _iccProfile.empty() && _xmp.empty();
}

void Metadata::clearXMP() {
  if (!_xmp.empty()) {
    _xmp.clear();
  }
}

const metadata::Entries& Metadata::entries() const {
  return _entries;
}

const metadata::ICCProfile& Metadata::iccProfile() const {
  return _iccProfile;
}

const std::string& Metadata::xmp() const {
  return _xmp;
}

metadata::Entries& Metadata::entries() {
  return _entries;
}

metadata::ICCProfile& Metadata::iccProfile() {
  return _iccProfile;
}

std::string& Metadata::xmp() {
  return _xmp;
}

void Metadata::merge(const Metadata& other) {
  _entries.merge(other._entries);

  if (!other._iccProfile.empty()) {
    _iccProfile = other._iccProfile;
  }
  if (!other._xmp.empty()) {
    _xmp = other._xmp;
  }
}

std::string Metadata::string() const {
  return "{}";
}

bool Metadata::operator==(const Metadata& rhs) const {
  return _entries == rhs._entries && _iccProfile == rhs._iccProfile &&
      _xmp == rhs._xmp;
}

bool Metadata::operator!=(const Metadata& rhs) const {
  return !(*this == rhs);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
