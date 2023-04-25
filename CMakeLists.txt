cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(RockLib)

if(NOT "${CMAKE_TOOLCHAIN_FILE}")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(rock3d LANGUAGES CXX)

### Rock3D engine ##############################################################

find_package(bgfx CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(ROCK3D_SOURCES
    "src/assets.cpp"
    "src/engine.cpp"
    "src/event.cpp"
    "src/level.cpp"
    "src/r3d/render.cpp"
    "src/r3d/textures.cpp"
    "src/random.cpp"
    "src/renderUtils.cpp"
    "src/vendor/mapbox/earcut.hpp"
    "src/vendor/stb_rect_pack.cpp"
    "src/vendor/stb_rect_pack.h")

if(WIN32)
    list(APPEND ROCK3D_SOURCES "src/platform_win32.cpp")
endif()

set(ROCK3D_HEADERS
    "include/rock3d/assets.h"
    "include/rock3d/engine.h"
    "include/rock3d/event.h"
    "include/rock3d/level.h"
    "include/rock3d/mathlib.h"
    "include/rock3d/platform.h"
    "include/rock3d/renderUtils.h"
    "include/rock3d/random.h"
    "include/rock3d/rock3d.h"
    "include/rock3d/util.h"
    "include/rock3d/nonstd/expected.hpp"
    "include/rock3d/nonstd/scope.hpp"
    "include/rock3d/nonstd/span.hpp"
    "include/rock3d/r3d/render.h"
    "include/rock3d/r3d/textures.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ROCK3D_SOURCES} ${ROCK3D_HEADERS})

if(MSVC)
    checked_add_compile_flag(ROCK3D_COMPILE_OPTIONS "/permissive-" ROCK3D_HAS_PERMISSIVE)
    checked_add_compile_flag(ROCK3D_COMPILE_OPTIONS "/Zc:__cplusplus" ROCK3D_HAS_CPLUSPLUS)
endif()

add_library(rock3d STATIC ${ROCK3D_SOURCES} ${ROCK3D_HEADERS})
target_compile_options(rock3d PRIVATE /W4 ${ROCK3D_COMPILE_OPTIONS})
target_compile_features(rock3d PUBLIC cxx_std_17)
target_include_directories(rock3d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(rock3d PUBLIC bgfx::bgfx bgfx::bx bgfx::bimg)
target_link_libraries(rock3d PUBLIC fmt::fmt)
target_link_libraries(rock3d PUBLIC glm::glm)
target_link_libraries(rock3d PUBLIC JsonCpp::JsonCpp)
target_link_libraries(rock3d
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

### RockED editor ##############################################################

add_executable(rocked WIN32
    "rocked/rocked.cpp"         "rocked/rocked.h"
    "rocked/imgui_rock3d.cpp"   "rocked/imgui_rock3d.h")
target_compile_features(rocked PRIVATE cxx_std_17)

target_link_libraries(rocked PRIVATE rock3d)
target_link_libraries(rocked PRIVATE imgui::imgui)
