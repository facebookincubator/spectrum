cmake_minimum_required(VERSION 3.6.0)
project(spectrumfbjni CXX)

file(GLOB fbjni_SOURCES
  cxx/fbjni/*.cpp
  cxx/fbjni/detail/*.cpp
  cxx/lyra/*.cpp
)
add_library(spectrumfbjni SHARED
  ${fbjni_SOURCES}
)

target_compile_options(spectrumfbjni PRIVATE
  -fno-omit-frame-pointer
  -fexceptions
  -O3
  -Wall
  -std=c++11
  -frtti
  -ffunction-sections
  -DDISABLE_CPUCAP
  -DDISABLE_XPLAT
  -DNDEBUG
)

target_include_directories(spectrumfbjni PUBLIC
  cxx
)

target_link_libraries(spectrumfbjni
  android
  log
)
