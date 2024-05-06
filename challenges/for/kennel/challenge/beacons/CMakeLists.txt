cmake_minimum_required(VERSION 3.25)
project(nothinghered)
set(CMAKE_AUTOGEN_VERBOSE on)
set(CMAKE_CXX_STANDARD 26)

set(SOURCE_FILES main.cpp beacon.cpp beacon.h
	consts.h
	executer.cpp
	executer.h
	util.h
)

add_executable(nothinghered ${SOURCE_FILES})
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

target_link_libraries(nothinghered /usr/lib/x86_64-linux-gnu/libMagick++-6.Q16.so)
