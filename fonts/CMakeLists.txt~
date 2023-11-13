cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(fonts C CXX ASM)

add_library(fonts
	fonts.h
	fonts.c
	)

target_include_directories(fonts PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(fonts)
	
