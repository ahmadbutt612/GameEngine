cmake_minimum_required(VERSION 3.20)
project("3D Game Engine" C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/lib/cmake/SFML")
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(3D_Game_Engine src/main.cpp src/glad.c src/Camera.cpp)
target_include_directories(3D_Game_Engine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(3D_Game_Engine PRIVATE SFML::Graphics SFML::Window SFML::System opengl32)

set_target_properties(3D_Game_Engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)