/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

// Derived from this Gist by Richard Gale:
//     https://gist.github.com/RichardGale/6e2b74bc42b3005e08397236e4be0fd0

#pragma once

namespace rocked
{

class RockImGui final
{
  protected:
    uint8_t m_view = 255;
    bgfx::TextureHandle m_cFontTexture = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle m_cShaderHandle = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_cAttribLocationTex = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout m_cVertexLayout;

    auto RockImGui::CreateFontsTexture() -> bool;

  public:
    RockImGui() {}
    virtual ~RockImGui() {}
    ROCK3D_NOCOPY(RockImGui);

    auto Init(const int view) -> void;
    auto Shutdown() -> void;
    auto HandleEvent(const rock3d::event_t &cEvent) -> bool;
    auto NewFrame() -> void;
    auto RenderDrawLists(ImDrawData *draw_data) -> void;

    // Use if you want to reset your rendering device without losing ImGui state.
    auto CreateDeviceObjects() -> void;
    auto InvalidateDeviceObjects() -> void;
};

} // namespace rocked
