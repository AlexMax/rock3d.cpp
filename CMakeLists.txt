cmake_minimum_required(VERSION 3.21)

project(rock3d LANGUAGES CXX)

find_package(bgfx CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(ROCK3D_SOURCES
    "rock3d/engine.cpp"
    "rock3d/resource.cpp")

if(WIN32)
    list(APPEND ROCK3D_SOURCES "rock3d/platform_win32.cpp")
endif()

set(ROCK3D_HEADERS
    "rock3d/bit.h"
    "rock3d/engine.h"
    "rock3d/event.h"
    "rock3d/mathlib.h"
    "rock3d/platform.h"
    "rock3d/random.h"
    "rock3d/resource.h"
    "rock3d/rock3d.h")

add_library(rock3d STATIC ${ROCK3D_SOURCES} ${ROCK3D_HEADERS})
target_compile_features(rock3d PRIVATE cxx_std_17)
target_include_directories(rock3d PUBLIC "rock3d" "rock3d/vendor")

target_link_libraries(rock3d PUBLIC bgfx::bx bgfx::bgfx bgfx::bimg bgfx::bimg_decode)
target_link_libraries(rock3d PUBLIC fmt::fmt)
target_link_libraries(rock3d PUBLIC glm::glm)
target_link_libraries(rock3d
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

add_executable(rocked WIN32
    "rocked/main.cpp"
    "rocked/imgui_impl_bgfx.cpp"    "rocked/imgui_impl_bgfx.h"
    "rocked/imgui_impl_sdl2.cpp"    "rocked/imgui_impl_sdl2.h")
target_compile_features(rocked PRIVATE cxx_std_17)

target_link_libraries(rocked PRIVATE rock3d)
target_link_libraries(rocked PRIVATE imgui::imgui)

# ------------------------------------------------------------------------------

function(add_shaders AS_TARGET)
    cmake_parse_arguments("AS" "" "OUTDIR" "PROFILES;VERTS;FRAGS" ${ARGN})
    foreach(AS_PROFILE IN LISTS AS_PROFILES)
        foreach(AS_VERT IN LISTS AS_VERTS)
            get_filename_component(AS_VERTFILE "${AS_VERT}" NAME)
            set(AS_OUTFILE "${AS_OUTDIR}/${AS_PROFILE}/${AS_VERTFILE}.bin")
            add_custom_command(
                OUTPUT "${AS_OUTFILE}"
                COMMAND bgfx::shaderc
                -i "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include/bgfx"
                -f "${AS_VERT}" -o "${AS_OUTFILE}" --type v --profile "${AS_PROFILE}")
            list(APPEND AS_DEPENDS "${AS_OUTFILE}")
        endforeach()

        foreach(AS_FRAG IN LISTS AS_FRAGS)
            get_filename_component(AS_FRAGFILE "${AS_FRAG}" NAME)
            set(AS_OUTFILE "${AS_OUTDIR}/${AS_PROFILE}/${AS_FRAGFILE}.bin")
            add_custom_command(
                OUTPUT "${AS_OUTFILE}"
                COMMAND bgfx::shaderc
                -i "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include/bgfx"
                -f "${AS_FRAG}" -o "${AS_OUTFILE}" --type f --profile "${AS_PROFILE}")
            list(APPEND AS_DEPENDS "${AS_OUTFILE}")
        endforeach()
    endforeach()

    add_custom_target("${AS_TARGET}" DEPENDS ${AS_DEPENDS})
endfunction()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(ROCKED_SHADERS_PROFILE "metal")
else()
    set(ROCKED_SHADERS_PROFILE "spirv15-12") # Vulkan 1.2.
endif()

add_shaders(rocked_shaders
    OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/rocked/shaders"
    PROFILES ${ROCKED_SHADERS_PROFILE}
    VERTS "${CMAKE_CURRENT_SOURCE_DIR}/rocked/shaders/vs_ocornut_imgui.sc"
    FRAGS "${CMAKE_CURRENT_SOURCE_DIR}/rocked/shaders/fs_ocornut_imgui.sc")