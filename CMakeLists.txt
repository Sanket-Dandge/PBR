cmake_minimum_required(VERSION 3.18)
project(PBR LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wpedantic)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(assimp_DIR "/usr/local/lib/cmake/assimp-6.0/")

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

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# Executable name
set(PROJECT_NAME_EXEC pbr)

# Add executable
add_executable(${PROJECT_NAME_EXEC} ${sources})

# Link libraries
target_link_libraries(${PROJECT_NAME_EXEC}
    glad
    glfw
    assimp
    imgui
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)
