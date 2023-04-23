/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Alex Mayfield <alexmax2742@gmail.com>
 */

namespace rock3d
{

/**
 * @brief Compile a shader program.
 *
 * @param strShaderDir Path to the directory containing the shader source.
 *                     For example: "rock3d/r3d/shaders/world".
 * @return A shader program handle.  Compile errors are fatal.
 */
auto ShaderCompileProgram(const std::string_view strShaderDir) -> bgfx::ProgramHandle;

} // namespace rock3d
