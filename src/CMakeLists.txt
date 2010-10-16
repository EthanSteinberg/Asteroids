set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(blocksexe main.cpp util.cpp asteroids.cpp menu.cpp game.cpp)
target_link_libraries(blocksexe stdc++ ${FREETYPE_LIBRARIES} ${ImageMagick_LIBRARIES} YAJL GLEW ${Boost_LIBRARIES} SFML_WINDOW SFML_SYSTEM)
