PROJECT(FALLINGBLOCKS)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/arch)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -pedantic -Werror")
set(CMAKE_BUILD_TYPE Debug)

add_library(SFML SHARED IMPORTED)
set_target_properties(SFML PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/SFML-1.6/lib/libsfml-system.so.1.6)
include_directories(${CMAKE_SOURCE_DIR}/lib/SFML-1.6/include)

find_package(OpenGL)
include_directories(OPENGL_INCLUDE_DIR)

find_package(X11)
include_directories(X11_INCLUDE_DIR)

find_package(Boost 1.36.0 COMPONENTS thread)
include_directories(BOOST_INCLUDE_DIR)

add_subdirectory(src)
