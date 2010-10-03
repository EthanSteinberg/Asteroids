PROJECT(FALLINGBLOCKS)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/arch)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow")
set(CMAKE_BUILD_TYPE Debug)

add_library(SFML_SYSTEM SHARED IMPORTED)
set_target_properties(SFML_SYSTEM PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/SFML-1.6/lib/libsfml-system.so.1.6)

add_library(SFML_WINDOW SHARED IMPORTED)
set_target_properties(SFML_WINDOW PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/SFML-1.6/lib/libsfml-window.so.1.6)

include_directories(${CMAKE_SOURCE_DIR}/lib/SFML-1.6/include)

add_library(GLEW SHARED IMPORTED)
set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION /usr/lib64/libGLEW.so)

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(Boost 1.36.0 COMPONENTS thread)
include_directories(${BOOST_INCLUDE_DIR})

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

add_subdirectory(src)
