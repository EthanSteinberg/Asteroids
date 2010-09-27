PROJECT(FALLINGBLOCKS)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/arch)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

add_library(GLEW STATIC IMPORTED)
set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glew-1.5.6/lib/libGLEW.a)

find_package(OpenGL)
include_directories(OPENGL_INCLUDE_DIR)

find_package(X11)
include_directories(X11_INCLUDE_DIR)

find_package(Boost 1.36.0 COMPONENTS thread)
include_directories(BOOST_INCLUDE_DIR)

add_subdirectory(src)
