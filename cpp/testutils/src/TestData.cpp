// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "TestData.h"

#include <cstddef>
#include <stdexcept>
#include <string>

#include <boost/filesystem.hpp>

namespace facebook {
namespace spectrum {
namespace testdata {

std::string Path::normalized() const {
  static constexpr int maxFoldersUp = 5;
  boost::filesystem::path pathCandidate(path.toString());

  for (int i = 0; i < maxFoldersUp; i++) {
    if (boost::filesystem::exists(pathCandidate)) {
      return pathCandidate.string();
    }

    // anchor relative path one level higher
    pathCandidate = boost::filesystem::path{".."} / pathCandidate;
  }

  throw std::runtime_error("Could not find testdata");
}

} // namespace testdata
} // namespace spectrum
} // namespace facebook
