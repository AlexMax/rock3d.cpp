/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

// Derived from this Gist by Richard Gale:
//     https://gist.github.com/RichardGale/6e2b74bc42b3005e08397236e4be0fd0

#include "rocked.h"

// BGFX/BX
#include "bx/math.h"
#include "bx/timer.h"

namespace rocked
{

// *****************************************************************************

auto RockImGui::CreateFontsTexture() -> bool
{
    // Build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Upload texture to graphics system
    m_cFontTexture = bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::BGRA8, 0,
                                           bgfx::copy(pixels, width * height * 4));

    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t)m_cFontTexture.idx;

    return true;
}

// *****************************************************************************

auto RockImGui::Init(const int view) -> void
{
    m_view = (uint8_t)(view & 0xff);
}

// *****************************************************************************

auto RockImGui::Shutdown() -> void
{
    InvalidateDeviceObjects();
}

// *****************************************************************************

auto RockImGui::NewFrame() -> void
{
    if (!bgfx::isValid(m_cFontTexture))
    {
        CreateDeviceObjects();
    }
}

// *****************************************************************************

auto RockImGui::RenderDrawLists(ImDrawData *draw_data) -> void
{
    // This is the main rendering function that you have to implement and call after
    // ImGui::Render(). Pass ImGui::GetDrawData() to this function.
    // Note: If text or lines are blurry when integrating ImGui into your engine,
    // in your Render function, try translating your projection matrix by
    // (0.5f,0.5f) or (0.375f,0.375f)

    // Avoid rendering when minimized, scale coordinates for retina displays
    // (screen coordinates != framebuffer coordinates)
    ImGuiIO &io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
    {
        return;
    }

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Setup render state: alpha-blending enabled, no face culling,
    // no depth testing, scissor enabled
    uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA |
                     BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

    const bgfx::Caps *caps = bgfx::getCaps();

    // Setup viewport, orthographic projection matrix
    float ortho[16];
    bx::mtxOrtho(ortho, 0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(m_view, NULL, ortho);
    bgfx::setViewRect(m_view, 0, 0, (uint16_t)fb_width, (uint16_t)fb_height);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        uint32_t numVertices = (uint32_t)cmd_list->VtxBuffer.size();
        uint32_t numIndices = (uint32_t)cmd_list->IdxBuffer.size();

        if ((numVertices != bgfx::getAvailTransientVertexBuffer(numVertices, m_cVertexLayout)) ||
            (numIndices != bgfx::getAvailTransientIndexBuffer(numIndices)))
        {
            // not enough space in transient buffer, quit drawing the rest...
            break;
        }

        bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_cVertexLayout);
        bgfx::allocTransientIndexBuffer(&tib, numIndices);

        ImDrawVert *verts = (ImDrawVert *)tvb.data;
        memcpy(verts, cmd_list->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

        ImDrawIdx *indices = (ImDrawIdx *)tib.data;
        memcpy(indices, cmd_list->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                const uint16_t xx = (uint16_t)bx::max(pcmd->ClipRect.x, 0.0f);
                const uint16_t yy = (uint16_t)bx::max(pcmd->ClipRect.y, 0.0f);
                bgfx::setScissor(xx, yy, (uint16_t)bx::min(pcmd->ClipRect.z, 65535.0f) - xx,
                                 (uint16_t)bx::min(pcmd->ClipRect.w, 65535.0f) - yy);

                bgfx::setState(state);
                bgfx::TextureHandle texture = {(uint16_t)((intptr_t)pcmd->TextureId & 0xffff)};
                bgfx::setTexture(0, m_cAttribLocationTex, texture);
                bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
                bgfx::setIndexBuffer(&tib, pcmd->IdxOffset, pcmd->ElemCount);
                bgfx::submit(m_view, m_cShaderHandle);
            }
        }
    }
}

// *****************************************************************************

auto RockImGui::CreateDeviceObjects() -> bool
{
    const auto maybeVert = rock3d::GetResource().ReadToBuffer("shaders/spirv15-12/vs_ocornut_imgui.sc.bin");
    const auto maybeFrag = rock3d::GetResource().ReadToBuffer("shaders/spirv15-12/fs_ocornut_imgui.sc.bin");
    if (!maybeVert.has_value() || !maybeFrag.has_value())
    {
        return false;
    }

    // [LM] AFAICT, these are freed by bgfx and there's no standalone free function.
    const bgfx::Memory *vert = bgfx::copy(maybeVert.value().data(), maybeVert.value().size());
    const bgfx::Memory *frag = bgfx::copy(maybeFrag.value().data(), maybeFrag.value().size());

    bgfx::RendererType::Enum type = bgfx::getRendererType();
    m_cShaderHandle = bgfx::createProgram(bgfx::createShader(vert), bgfx::createShader(frag), true);

    m_cVertexLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

    m_cAttribLocationTex = bgfx::createUniform("g_AttribLocationTex", bgfx::UniformType::Sampler);

    CreateFontsTexture();

    return true;
}

// *****************************************************************************

auto RockImGui::InvalidateDeviceObjects() -> void
{
    bgfx::destroy(m_cAttribLocationTex);
    bgfx::destroy(m_cShaderHandle);

    if (isValid(m_cFontTexture))
    {
        bgfx::destroy(m_cFontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        m_cFontTexture.idx = bgfx::kInvalidHandle;
    }
}

} // namespace rocked
