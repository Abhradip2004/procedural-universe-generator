cmake_minimum_required(VERSION 3.10)

project(UniverseSim)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(
    external/glad/include
    external/imgui
    external/imgui/backends
)

# GLAD
set(GLAD_SRC
    external/glad/src/glad.c
)

# IMGUI
set(IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(UniverseSim
    src/main.cpp
    src/Universe.cpp
    ${GLAD_SRC}
    ${IMGUI_SRC}
)

target_link_libraries(UniverseSim
    OpenGL::GL
    glfw
)