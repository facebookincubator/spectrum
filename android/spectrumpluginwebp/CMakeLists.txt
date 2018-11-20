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

set(spectrumpluginwebp_DIR ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp)
file(GLOB spectrumpluginwebp_SOURCES
  ${spectrumpluginwebp_DIR}/spectrumjni/plugins/JniSpectrumPluginWebp.cpp
  ${spectrumpluginwebp_DIR}/spectrumjni/OnLoad.cpp
)

add_library(spectrumpluginwebp SHARED
  ${spectrumpluginwebp_SOURCES}
)

target_compile_options(spectrumpluginwebp PRIVATE
  -DSPECTRUM_OSS
  -fexceptions
)

target_include_directories(spectrumpluginwebp PRIVATE
  ${spectrumpluginwebp_DIR}
)

set(EXTERNAL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../androidLibs/third-party/)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
file(MAKE_DIRECTORY ${BUILD_DIR})

set(glog_DIR ${EXTERNAL_DIR}/glog/merge/glog)
set(glog_BUILD_DIR ${BUILD_DIR}/glog/${ANDROID_ABI})
add_subdirectory(${glog_DIR} ${glog_BUILD_DIR})

set(folly_DIR ${EXTERNAL_DIR}/folly/merge/folly)
set(folly_BUILD_DIR ${BUILD_DIR}/folly/${ANDROID_ABI})
add_subdirectory(${folly_DIR} ${folly_BUILD_DIR})

set(libwebp_DIR ${EXTERNAL_DIR}/libwebp/merge/libwebp)
set(libwebp_BUILD_DIR ${BUILD_DIR}/libwebp/${ANDROID_ABI})
add_subdirectory(${libwebp_DIR} ${libwebp_BUILD_DIR})

set(spectrumcpp_DIR ${CMAKE_CURRENT_LIST_DIR}/../../cpp/)
set(spectrumcpp_BUILD_DIR ${BUILD_DIR}/spectrumcpp/${ANDROID_ABI})
add_subdirectory(${spectrumcpp_DIR} ${spectrumcpp_BUILD_DIR})

set(fbjni_DIR ${CMAKE_CURRENT_LIST_DIR}/../../androidLibs/fbjni/)
set(fbjni_BUILD_DIR ${BUILD_DIR}/fbjni/${ANDROID_ABI})
add_subdirectory(${fbjni_DIR} ${fbjni_BUILD_DIR})

target_link_libraries(spectrumpluginwebp
  spectrumfbjni
  spectrumcpp
  spectrumwebpcpp
)
