cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(RockLib)

if(NOT "${CMAKE_TOOLCHAIN_FILE}")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

project(rock3d LANGUAGES CXX)

find_package(bgfx CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(ROCK3D_SOURCES
    "rock3d/engine.cpp"
    "rock3d/event.cpp"
    "rock3d/renderUtils.cpp"
    "rock3d/random.cpp"
    "rock3d/resource.cpp"
    "rock3d/r3d/render.cpp"
    "rock3d/r3d/textures.cpp"
    "rock3d/vendor/stb_rect_pack.cpp")

if(WIN32)
    list(APPEND ROCK3D_SOURCES "rock3d/platform_win32.cpp")
endif()

set(ROCK3D_HEADERS
    "rock3d/bit.h"
    "rock3d/engine.h"
    "rock3d/event.h"
    "rock3d/mathlib.h"
    "rock3d/platform.h"
    "rock3d/renderUtils.h"
    "rock3d/random.h"
    "rock3d/resource.h"
    "rock3d/rock3d.h"
    "rock3d/r3d/render.h"
    "rock3d/r3d/textures.h")

if(MSVC)
    checked_add_compile_flag(ROCK3D_COMPILE_OPTIONS "/permissive-" ROCK3D_HAS_PERMISSIVE)
    checked_add_compile_flag(ROCK3D_COMPILE_OPTIONS "/Zc:__cplusplus" ROCK3D_HAS_CPLUSPLUS)
endif()

add_library(rock3d STATIC ${ROCK3D_SOURCES} ${ROCK3D_HEADERS})
target_compile_options(rock3d PRIVATE /W4 ${ROCK3D_COMPILE_OPTIONS})
target_compile_features(rock3d PUBLIC cxx_std_17)
target_include_directories(rock3d PUBLIC "rock3d" "rock3d/vendor")

target_link_libraries(rock3d PUBLIC bgfx::bgfx bgfx::bx bgfx::bimg)
target_link_libraries(rock3d PUBLIC fmt::fmt)
target_link_libraries(rock3d PUBLIC glm::glm)
target_link_libraries(rock3d PUBLIC JsonCpp::JsonCpp)
target_link_libraries(rock3d
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

add_executable(rocked WIN32
    "rocked/rocked.cpp"         "rocked/rocked.h"
    "rocked/imgui_rock3d.cpp"   "rocked/imgui_rock3d.h")
target_compile_features(rocked PRIVATE cxx_std_17)

target_link_libraries(rocked PRIVATE rock3d)
target_link_libraries(rocked PRIVATE imgui::imgui)
