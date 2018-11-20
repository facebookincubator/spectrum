cmake_minimum_required(VERSION 3.6.0)
project(libwebp C)

set(WEBP_NEON_EXTENSION c)
file(GLOB libwebp_SOURCES
  src/dec/*.c
  src/demux/*.c
  src/dsp/*.c
  src/enc/*.c
  src/mux/*.c
  src/utils/*.c
)
add_library(libwebp STATIC
  ${libwebp_SOURCES}
)

target_compile_definitions(libwebp PRIVATE
  -DANDROID
  -DHAVE_MALLOC_H
  -DHAVE_PTHREAD
  -DWEBP_USE_THREAD
  -DWEBP_FORCE_ALIGNED
)

target_compile_options(libwebp PRIVATE
  -finline-functions
  -ffast-math
  -ffunction-sections
  -fdata-sections
)

target_include_directories(libwebp PUBLIC
  .
  src
)

add_library(cpufeatures STATIC
  ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
)

target_link_libraries(libwebp PUBLIC
  cpufeatures
)
