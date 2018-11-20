cmake_minimum_required(VERSION 3.6.0)
project(libpng C)

add_library(libpng STATIC
  png.c
	pngerror.c
	pngget.c
	pngmem.c
	pngpread.c
	pngread.c
	pngrio.c
	pngrtran.c
	pngrutil.c
	pngset.c
	pngtrans.c
	pngwio.c
	pngwrite.c
	pngwtran.c
  pngwutil.c
  arm/arm_init.c
  arm/filter_neon_intrinsics.c
  arm/filter_neon.S
)

target_compile_options(libpng PRIVATE
)

target_include_directories(libpng PUBLIC
  .
)

target_link_libraries(libpng PUBLIC
  z #zlib
)
