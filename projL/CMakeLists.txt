cmake_minimum_required(VERSION 3.10)
project(glRoom VERSION 1.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

add_executable(room src/main.cpp)

target_link_libraries(room
    glfw
    OpenGL::GL
)

