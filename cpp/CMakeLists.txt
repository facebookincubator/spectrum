# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

#
# Main target: spectrumcpp
#

cmake_minimum_required(VERSION 3.6.0)
project(spectrumcpp CXX)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB spectrumcpp_SOURCES
  Spectrum/*.cpp
  Spectrum/codecs/*.cpp
  Spectrum/codecs/bitmap/*.cpp
  Spectrum/codecs/isobmff/*.cpp
  Spectrum/core/*.cpp
  Spectrum/core/decisions/*.cpp
  Spectrum/core/matchers/*.cpp
  Spectrum/core/recipes/*.cpp
  Spectrum/core/proc/*.cpp
  Spectrum/core/proc/legacy/*.cpp
  Spectrum/core/utils/*.cpp
  Spectrum/image/*.cpp
  Spectrum/image/metadata/*.cpp
  Spectrum/io/*.cpp
  Spectrum/requirements/*.cpp
)

add_library(spectrumcpp SHARED
  ${spectrumcpp_SOURCES}
)

target_compile_options(spectrumcpp PUBLIC
  -DSPECTRUM_TARGET_ANDROID
  -fexceptions
)

target_include_directories(spectrumcpp PUBLIC
  .
)

target_link_libraries(spectrumcpp
  folly
  glog
)

#
# Plugin target: spectrumjpegcpp
#

project(spectrumjpegcpp CXX)

file(GLOB spectrumjpegcpp_SOURCES "spectrum/plugins/jpeg/*.cpp")
add_library(spectrumjpegcpp STATIC
  ${spectrumjpegcpp_SOURCES}
)

target_include_directories(spectrumjpegcpp PUBLIC
  .
)

target_link_libraries(spectrumjpegcpp
  mozjpeg
  spectrumcpp
)

#
# Plugin target: spectrumpngcpp
#

project(spectrumpngcpp CXX)

file(GLOB spectrumpngcpp_SOURCES "spectrum/plugins/png/*.cpp")
add_library(spectrumpngcpp STATIC
  ${spectrumpngcpp_SOURCES}
)

target_include_directories(spectrumpngcpp PUBLIC
  .
)

target_link_libraries(spectrumpngcpp
  libpng
  spectrumcpp
)

#
# Plugin target: spectrumwebpcpp
#

project(spectrumwebpcpp CXX)

file(GLOB spectrumwebpcpp_SOURCES "spectrum/plugins/webp/*.cpp")
add_library(spectrumwebpcpp STATIC
  ${spectrumwebpcpp_SOURCES}
)

target_include_directories(spectrumwebpcpp PUBLIC
  .
)

target_link_libraries(spectrumwebpcpp
  libwebp
  spectrumcpp
)
