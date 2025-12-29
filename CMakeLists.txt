cmake_minimum_required(VERSION 3.14)
project(Stratum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW 
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG latest)
FetchContent_MakeAvailable(glfw)

# GLM 
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG master)
FetchContent_MakeAvailable(glm)

# ImGui (GUI) 
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG docking)
FetchContent_MakeAvailable(imgui)

# Your code
set(APP_SOURCES
    src/main.cpp
    src/glad.c
    src/DataLoader.cpp
    src/Window.cpp
)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(Stratum ${APP_SOURCES} ${IMGUI_SOURCES})

target_include_directories(Stratum PUBLIC 
    include 
    ${glfw_SOURCE_DIR}/include 
    ${glm_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(Stratum PRIVATE glfw glm)