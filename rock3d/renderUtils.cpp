/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Alex Mayfield <alexmax2742@gmail.com>
 */

#include "../rock3d.h"

#include <algorithm>

#include "bgfx/bgfx.h"

namespace rock3d
{

auto ShaderCompileProgram(const std::string_view strShaderDir) -> bgfx::ProgramHandle
{
    std::string prefix{"shaders/spirv15-12/"};
    std::string shaderDir(strShaderDir);
    std::replace(shaderDir.begin(), shaderDir.end(), '/', '_');

    std::string vertFile = fmt::format("{}{}_vert.sc.bin", prefix, shaderDir);
    const auto maybeVert = rock3d::GetResource().ReadToBuffer(vertFile);
    if (!maybeVert.has_value())
    {
        rock3d::GetPlatform().FatalError(fmt::format("Missing shader file: {}", vertFile));
    }

    std::string fragFile = fmt::format("{}{}_frag.sc.bin", prefix, shaderDir);
    const auto maybeFrag = rock3d::GetResource().ReadToBuffer(fragFile);
    if (!maybeFrag.has_value())
    {
        rock3d::GetPlatform().FatalError(fmt::format("Missing shader file: {}", fragFile));
    }

    // [LM] AFAICT, these are freed by bgfx and there's no standalone free function.
    const bgfx::Memory *vert = bgfx::copy(maybeVert.value().data(), maybeVert.value().size());
    const bgfx::Memory *frag = bgfx::copy(maybeFrag.value().data(), maybeFrag.value().size());

    bgfx::RendererType::Enum type = bgfx::getRendererType();
    bgfx::ProgramHandle handle = bgfx::createProgram(bgfx::createShader(vert), bgfx::createShader(frag), true);
    if (handle.idx == bgfx::kInvalidHandle)
    {
        rock3d::GetPlatform().FatalError(fmt::format("Could not compile shader: {}", strShaderDir));
    }
    return handle;
}

} // namespace rock3d
