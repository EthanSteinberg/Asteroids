set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(blocksexe main.cpp blocks.cpp)
target_link_libraries(blocksexe stdc++  ${Boost_LIBRARIES} SFML_WINDOW SFML_SYSTEM)
