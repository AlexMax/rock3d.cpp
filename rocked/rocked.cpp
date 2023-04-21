/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#include "rocked.h"

namespace rocked
{

class EditorApp final : public rock3d::App
{
    static constexpr rock3d::App::config_s CONFIG{1000 / 60, 1000 / 4};

    RockImGui m_cRockImGui;

  public:
    auto Config() -> const rock3d::App::config_s & override
    {
        return CONFIG;
    }

    auto Init() -> void override
    {
        rock3d::GetResource().AddPath(std::string(rock3d::GetPlatform().GetBasePath()) + "../rocked");
        rock3d::GetResource().AddPath(std::string(rock3d::GetPlatform().GetBasePath()) + "rocked");

        ImGui::CreateContext();

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        main_viewport->PlatformHandleRaw = rock3d::GetPlatform().WindowHandle();

        m_cRockImGui.Init(0);
    }

    auto Tick(const tickParams_s &cParams) -> void override
    {
    }

    auto Render(const renderParams_s &cParams) -> void override
    {
        const glm::ivec2 res = rock3d::GetPlatform().WindowResolution();
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize.x = res.x;
        io.DisplaySize.y = res.y;

        m_cRockImGui.NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        m_cRockImGui.RenderDrawLists(ImGui::GetDrawData());

        bgfx::frame();
    }

    auto Shutdown() -> void override
    {
        m_cRockImGui.Shutdown();
    }
};

} // namespace rocked

std::unique_ptr<rock3d::App> g_cDefinedApp{new rocked::EditorApp()};

// DEFINE_APPLICATION(rocked::EditorApp);
