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
