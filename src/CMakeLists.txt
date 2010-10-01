set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(blocksexe main.cpp blocks.cpp)
target_link_libraries(blocksexe stdc++ ${OPENGL_LIBRARIES} ${X11_LIBRARIES}
${Boost_LIBRARIES} SFML)
