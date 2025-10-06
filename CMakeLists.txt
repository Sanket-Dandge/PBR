cmake_minimum_required(VERSION 3.18)
project(PBR LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wpedantic)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# set(assimp_DIR "/usr/local/lib/cmake/assimp-6.0/")

# Sources
file(GLOB_RECURSE sources
    src/*
)

# Include dirs
include_directories(
    include
    external/glad
    external/stb
    ${assimp_INCLUDE_DIRS}
)

# Build glad
add_library(glad STATIC external/glad/glad.c)

# Find system OpenGL, GLFW, Assimp
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# Executable name
set(PROJECT_NAME_EXEC pbr)

# Add executable
add_executable(${PROJECT_NAME_EXEC} ${sources})

# Link libraries
target_link_libraries(${PROJECT_NAME_EXEC}
    glad
    glfw
    assimp
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)
