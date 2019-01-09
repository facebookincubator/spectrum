cmake_minimum_required(VERSION 3.6.0)
project(mozjpeg C)

set(can_use_assembler TRUE)
enable_language(ASM)

list(APPEND mozjpegsources
  jcapimin.c
  jcapistd.c
  jccoefct.c
  jccolor.c
  jcdctmgr.c
  jcext.c
  jchuff.c
  jcinit.c
  jcmainct.c
  jcmarker.c
  jcmaster.c
  jcomapi.c
  jcparam.c
  jcphuff.c
  jcprepct.c
  jcsample.c
  jctrans.c
  jdapimin.c
  jdapistd.c
  jdatadst.c
  jdatasrc.c
  jdcoefct.c
  jdcolor.c
  jddctmgr.c
  jdhuff.c
  jdinput.c
  jdmainct.c
  jdmarker.c
  jdmaster.c
  jdmerge.c
  jdphuff.c
  jdpostct.c
  jdsample.c
  jdtrans.c
  jerror.c
  jfdctflt.c
  jfdctfst.c
  jfdctint.c
  jidctflt.c
  jidctfst.c
  jidctint.c
  jidctred.c
  jquant1.c
  jquant2.c
  jutils.c
  jmemmgr.c
  jaricom.c
  jcarith.c
  jdarith.c
  transupp.c
  jmemnobs.c
)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7-a")
  message(STATUS "Building mozjpeg with armv7-a SIMD support")
  list(APPEND mozjpegsources
    simd/jsimd_arm.c
    simd/jsimd_arm_neon.S
  )
else()
  message(STATUS "Building mozjpeg without SIMD support")
  list(APPEND mozjpegsources
    jsimd_none.c
  )
endif()


add_library(mozjpeg STATIC ${mozjpegsources})

target_compile_options(mozjpeg PRIVATE
  -fno-omit-frame-pointer
  -fexceptions
  -DJPEG_LIB_VERSION=80
  -Wno-attributes
  -O3
  -ffunction-sections
)

target_include_directories(mozjpeg PUBLIC
  .
  ..
  simd
)
