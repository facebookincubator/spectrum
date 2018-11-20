cmake_minimum_required(VERSION 3.6.0)
project(folly CXX)

add_library(folly INTERFACE)

target_compile_definitions(folly INTERFACE
  -DFOLLY_NO_CONFIG
  -DFOLLY_HAVE_MEMRCHR
  -DFOLLY_MOBILE=1
  -DFOLLY_USE_LIBCPP=1
  -DFOLLY_HAVE_LIBJEMALLOC=0
  -DFOLLY_HAVE_PREADV=0
)

target_include_directories(folly INTERFACE
  .
)
