# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required (VERSION 3.6.0)
project(spectrum CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

set(spectrumtest_DIR ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp)
file(GLOB spectrumtest_SOURCES
    ${spectrumtest_DIR}/spectrumjni/*.cpp
)

add_library(spectrumtest SHARED
    ${spectrumtest_SOURCES}
)

target_compile_options(spectrumtest PRIVATE
    -DSPECTRUM_OSS
    -fexceptions
)

target_include_directories(spectrumtest PRIVATE
    ${spectrumtest_DIR}
)

set(EXTERNAL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../androidLibs/third-party/)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
file(MAKE_DIRECTORY ${BUILD_DIR})

set(spectrum_DIR ${CMAKE_CURRENT_LIST_DIR}/../)
set(spectrum_BUILD_DIR ${BUILD_DIR}/spectrumjni/${ANDROID_ABI})
add_subdirectory(${spectrum_DIR} ${spectrum_BUILD_DIR})

target_link_libraries(spectrumtest
  spectrumfbjni
  spectrumcpp
  spectrum
)
