cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH "C:/VSARM/sdk/pico/pico-sdk")
set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)
project(oled_display C CXX ASM)
pico_sdk_init()

add_subdirectory(fonts)

add_library(oled_display
	oled_display.h
	oled_display.c
	)

target_include_directories(oled_display PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(oled_display
	pico_stdlib
	hardware_i2c
	fonts
	)
