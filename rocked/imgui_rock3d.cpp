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

#define IMGUI_CONV_(imgui_key, rock_key)                                                                               \
    case rock_key:                                                                                                     \
        return imgui_key;                                                                                              \
        static_assert(true, "")

/**
 * @brief Convert rock3d key to ImGui key.
 */
constexpr static auto RockKeyToImGuiKey(rock3d::keyboardKey_e eKey) -> ImGuiKey
{
    switch (eKey)
    {
        IMGUI_CONV_(ImGuiKey_Tab, rock3d::KKEY_TAB);
        IMGUI_CONV_(ImGuiKey_LeftArrow, rock3d::KKEY_LEFT);
        IMGUI_CONV_(ImGuiKey_RightArrow, rock3d::KKEY_RIGHT);
        IMGUI_CONV_(ImGuiKey_UpArrow, rock3d::KKEY_UP);
        IMGUI_CONV_(ImGuiKey_DownArrow, rock3d::KKEY_DOWN);
        IMGUI_CONV_(ImGuiKey_PageUp, rock3d::KKEY_PAGEUP);
        IMGUI_CONV_(ImGuiKey_PageDown, rock3d::KKEY_PAGEDOWN);
        IMGUI_CONV_(ImGuiKey_Home, rock3d::KKEY_HOME);
        IMGUI_CONV_(ImGuiKey_End, rock3d::KKEY_END);
        IMGUI_CONV_(ImGuiKey_Insert, rock3d::KKEY_INSERT);
        IMGUI_CONV_(ImGuiKey_Delete, rock3d::KKEY_DELETE);
        IMGUI_CONV_(ImGuiKey_Backspace, rock3d::KKEY_BACKSPACE);
        IMGUI_CONV_(ImGuiKey_Space, rock3d::KKEY_SPACE);
        IMGUI_CONV_(ImGuiKey_Enter, rock3d::KKEY_RETURN);
        IMGUI_CONV_(ImGuiKey_Escape, rock3d::KKEY_ESCAPE);
        IMGUI_CONV_(ImGuiKey_LeftCtrl, rock3d::KKEY_LCTRL);
        IMGUI_CONV_(ImGuiKey_LeftShift, rock3d::KKEY_LSHIFT);
        IMGUI_CONV_(ImGuiKey_LeftAlt, rock3d::KKEY_LALT);
        IMGUI_CONV_(ImGuiKey_LeftSuper, rock3d::KKEY_LGUI);
        IMGUI_CONV_(ImGuiKey_RightCtrl, rock3d::KKEY_RCTRL);
        IMGUI_CONV_(ImGuiKey_RightShift, rock3d::KKEY_RSHIFT);
        IMGUI_CONV_(ImGuiKey_RightAlt, rock3d::KKEY_RALT);
        IMGUI_CONV_(ImGuiKey_RightSuper, rock3d::KKEY_RGUI);
        IMGUI_CONV_(ImGuiKey_Menu, rock3d::KKEY_APPLICATION);
        IMGUI_CONV_(ImGuiKey_0, rock3d::KKEY_0);
        IMGUI_CONV_(ImGuiKey_1, rock3d::KKEY_1);
        IMGUI_CONV_(ImGuiKey_2, rock3d::KKEY_2);
        IMGUI_CONV_(ImGuiKey_3, rock3d::KKEY_3);
        IMGUI_CONV_(ImGuiKey_4, rock3d::KKEY_4);
        IMGUI_CONV_(ImGuiKey_5, rock3d::KKEY_5);
        IMGUI_CONV_(ImGuiKey_6, rock3d::KKEY_6);
        IMGUI_CONV_(ImGuiKey_7, rock3d::KKEY_7);
        IMGUI_CONV_(ImGuiKey_8, rock3d::KKEY_8);
        IMGUI_CONV_(ImGuiKey_9, rock3d::KKEY_9);
        IMGUI_CONV_(ImGuiKey_A, rock3d::KKEY_a);
        IMGUI_CONV_(ImGuiKey_B, rock3d::KKEY_b);
        IMGUI_CONV_(ImGuiKey_C, rock3d::KKEY_c);
        IMGUI_CONV_(ImGuiKey_D, rock3d::KKEY_d);
        IMGUI_CONV_(ImGuiKey_E, rock3d::KKEY_e);
        IMGUI_CONV_(ImGuiKey_F, rock3d::KKEY_f);
        IMGUI_CONV_(ImGuiKey_G, rock3d::KKEY_g);
        IMGUI_CONV_(ImGuiKey_H, rock3d::KKEY_h);
        IMGUI_CONV_(ImGuiKey_I, rock3d::KKEY_i);
        IMGUI_CONV_(ImGuiKey_J, rock3d::KKEY_j);
        IMGUI_CONV_(ImGuiKey_K, rock3d::KKEY_k);
        IMGUI_CONV_(ImGuiKey_L, rock3d::KKEY_l);
        IMGUI_CONV_(ImGuiKey_M, rock3d::KKEY_m);
        IMGUI_CONV_(ImGuiKey_N, rock3d::KKEY_n);
        IMGUI_CONV_(ImGuiKey_O, rock3d::KKEY_o);
        IMGUI_CONV_(ImGuiKey_P, rock3d::KKEY_p);
        IMGUI_CONV_(ImGuiKey_Q, rock3d::KKEY_q);
        IMGUI_CONV_(ImGuiKey_R, rock3d::KKEY_r);
        IMGUI_CONV_(ImGuiKey_S, rock3d::KKEY_s);
        IMGUI_CONV_(ImGuiKey_T, rock3d::KKEY_t);
        IMGUI_CONV_(ImGuiKey_U, rock3d::KKEY_u);
        IMGUI_CONV_(ImGuiKey_V, rock3d::KKEY_v);
        IMGUI_CONV_(ImGuiKey_W, rock3d::KKEY_w);
        IMGUI_CONV_(ImGuiKey_X, rock3d::KKEY_x);
        IMGUI_CONV_(ImGuiKey_Y, rock3d::KKEY_y);
        IMGUI_CONV_(ImGuiKey_Z, rock3d::KKEY_z);
        IMGUI_CONV_(ImGuiKey_F1, rock3d::KKEY_F1);
        IMGUI_CONV_(ImGuiKey_F2, rock3d::KKEY_F2);
        IMGUI_CONV_(ImGuiKey_F3, rock3d::KKEY_F3);
        IMGUI_CONV_(ImGuiKey_F4, rock3d::KKEY_F4);
        IMGUI_CONV_(ImGuiKey_F5, rock3d::KKEY_F5);
        IMGUI_CONV_(ImGuiKey_F6, rock3d::KKEY_F6);
        IMGUI_CONV_(ImGuiKey_F7, rock3d::KKEY_F7);
        IMGUI_CONV_(ImGuiKey_F8, rock3d::KKEY_F8);
        IMGUI_CONV_(ImGuiKey_F9, rock3d::KKEY_F9);
        IMGUI_CONV_(ImGuiKey_F10, rock3d::KKEY_F10);
        IMGUI_CONV_(ImGuiKey_F11, rock3d::KKEY_F11);
        IMGUI_CONV_(ImGuiKey_F12, rock3d::KKEY_F12);
        IMGUI_CONV_(ImGuiKey_Apostrophe, rock3d::KKEY_QUOTE);
        IMGUI_CONV_(ImGuiKey_Comma, rock3d::KKEY_COMMA);
        IMGUI_CONV_(ImGuiKey_Minus, rock3d::KKEY_MINUS);
        IMGUI_CONV_(ImGuiKey_Period, rock3d::KKEY_PERIOD);
        IMGUI_CONV_(ImGuiKey_Slash, rock3d::KKEY_SLASH);
        IMGUI_CONV_(ImGuiKey_Semicolon, rock3d::KKEY_SEMICOLON);
        IMGUI_CONV_(ImGuiKey_Equal, rock3d::KKEY_EQUALS);
        IMGUI_CONV_(ImGuiKey_LeftBracket, rock3d::KKEY_LEFTBRACKET);
        IMGUI_CONV_(ImGuiKey_Backslash, rock3d::KKEY_BACKSLASH);
        IMGUI_CONV_(ImGuiKey_RightBracket, rock3d::KKEY_RIGHTBRACKET);
        IMGUI_CONV_(ImGuiKey_GraveAccent, rock3d::KKEY_BACKQUOTE);
        IMGUI_CONV_(ImGuiKey_CapsLock, rock3d::KKEY_CAPSLOCK);
        IMGUI_CONV_(ImGuiKey_ScrollLock, rock3d::KKEY_SCROLLLOCK);
        IMGUI_CONV_(ImGuiKey_NumLock, rock3d::KKEY_NUMLOCKCLEAR);
        IMGUI_CONV_(ImGuiKey_PrintScreen, rock3d::KKEY_PRINTSCREEN);
        IMGUI_CONV_(ImGuiKey_Pause, rock3d::KKEY_PAUSE);
        IMGUI_CONV_(ImGuiKey_Keypad0, rock3d::KKEY_KP_0);
        IMGUI_CONV_(ImGuiKey_Keypad1, rock3d::KKEY_KP_1);
        IMGUI_CONV_(ImGuiKey_Keypad2, rock3d::KKEY_KP_2);
        IMGUI_CONV_(ImGuiKey_Keypad3, rock3d::KKEY_KP_3);
        IMGUI_CONV_(ImGuiKey_Keypad4, rock3d::KKEY_KP_4);
        IMGUI_CONV_(ImGuiKey_Keypad5, rock3d::KKEY_KP_5);
        IMGUI_CONV_(ImGuiKey_Keypad6, rock3d::KKEY_KP_6);
        IMGUI_CONV_(ImGuiKey_Keypad7, rock3d::KKEY_KP_7);
        IMGUI_CONV_(ImGuiKey_Keypad8, rock3d::KKEY_KP_8);
        IMGUI_CONV_(ImGuiKey_Keypad9, rock3d::KKEY_KP_9);
        IMGUI_CONV_(ImGuiKey_KeypadDecimal, rock3d::KKEY_KP_PERIOD);
        IMGUI_CONV_(ImGuiKey_KeypadDivide, rock3d::KKEY_KP_DIVIDE);
        IMGUI_CONV_(ImGuiKey_KeypadMultiply, rock3d::KKEY_KP_MULTIPLY);
        IMGUI_CONV_(ImGuiKey_KeypadSubtract, rock3d::KKEY_KP_MINUS);
        IMGUI_CONV_(ImGuiKey_KeypadAdd, rock3d::KKEY_KP_PLUS);
        IMGUI_CONV_(ImGuiKey_KeypadEnter, rock3d::KKEY_KP_ENTER);
        IMGUI_CONV_(ImGuiKey_KeypadEqual, rock3d::KKEY_KP_EQUALS);
    default:
        return ImGuiKey_None;
    }
}

/**
 * @brief Convert rock3d mouse button to ImGui mouse button.
 */
constexpr static auto RockMouseButtonToImGuiMouseButton(rock3d::mouseButton_e eButton) -> int
{
    switch (eButton)
    {
        IMGUI_CONV_(ImGuiMouseButton_Left, rock3d::MBTN_LEFT);
        IMGUI_CONV_(ImGuiMouseButton_Right, rock3d::MBTN_RIGHT);
        IMGUI_CONV_(ImGuiMouseButton_Middle, rock3d::MBTN_MIDDLE);
    default:
        return -1;
    }
}

#undef IMGUI_CONV_

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

auto RockImGui::HandleEvent(const rock3d::event_t &cEvent) -> bool
{
    ImGuiIO &io = ImGui::GetIO();

    auto key = std::get_if<rock3d::eventKey_s>(&cEvent);
    if (key)
    {
        auto ikey = RockKeyToImGuiKey(key->key);
        if (ikey)
        {
            io.AddKeyEvent(RockKeyToImGuiKey(key->key), key->pressed);
            return true;
        }
        return false;
    }

    auto mouseMotion = std::get_if<rock3d::eventMouseMotion_s>(&cEvent);
    if (mouseMotion)
    {
        io.AddMousePosEvent(float(mouseMotion->abs.x), float(mouseMotion->abs.y));
        return true;
    }

    auto mouseButton = std::get_if<rock3d::eventMouseButton_s>(&cEvent);
    if (mouseButton)
    {
        auto ibtn = RockMouseButtonToImGuiMouseButton(mouseButton->button);
        if (ibtn >= 0)
        {
            io.AddMouseButtonEvent(ibtn, mouseButton->pressed);
            return true;
        }
        return false;
    }

    return false;
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
