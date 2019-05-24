cmake_minimum_required(VERSION 3.6.0)
project(glog CXX)

add_library(glog INTERFACE)

target_include_directories(glog INTERFACE
  ..
  glog-0.4.0/src
)
