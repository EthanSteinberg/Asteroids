set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(blocksexe main.cpp util.cpp 2dadventure.cpp)
target_link_libraries(blocksexe stdc++ ${ImageMagick_LIBRARIES} GLEW ${Boost_LIBRARIES} SFML_WINDOW SFML_SYSTEM)
