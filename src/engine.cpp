/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#include "rock3d/rock3d.h"

// *****************************************************************************

extern std::unique_ptr<rock3d::App> g_cDefinedApp;

// *****************************************************************************

namespace rock3d
{

class Engine final
{
    std::unique_ptr<App> m_pApp = nullptr;

  public:
    /**
     * @brief Construct the engine with the passed application.
     *
     * @param cApp Pointer to application class.  Takes ownership.
     */
    Engine(std::unique_ptr<App> cApp) : m_pApp(std::move(cApp)) {}

    // *************************************************************************

    auto Init(const args_t &nstrArgs) -> void
    {
        GetPlatform().Init(nstrArgs);
        m_pApp->Init(nstrArgs);
    }

    // *************************************************************************

    auto AppConfig() -> const App::config_s &
    {
        return m_pApp->Config();
    }

    // *************************************************************************

    auto Tick() -> void
    {
        const App::config_s &config = m_pApp->Config();

        // Glenn Fiedler's fixed timestep, with interpolation.
        uint64_t f = 0;
        uint64_t t = 0;

        uint64_t currentTime = GetPlatform().TimeMS();
        uint64_t accumulator = 0;

        for (;;)
        {
            // Feed events into the queue.
            GetPlatform().PumpEvents();

            // Handle events from the queue.
            rock3d::event_t ev;
            while (GetEventQueue().Poll(ev))
            {
                m_pApp->HandleEvent(ev);
            }

            // Figure out our desired frame time.
            uint64_t newTime = GetPlatform().TimeMS();
            uint64_t frameTime = newTime - currentTime;
            if (frameTime > config.qwMaxMS)
            {
                frameTime = config.qwMaxMS;
            }
            currentTime = newTime;

            // Add frametime to the accumulator.
            accumulator += frameTime;

            // Remove time from the accumulator by ticking frames.
            while (accumulator >= config.qwDeltaMS)
            {
                m_pApp->Tick(App::tickParams_s{f, t, config.qwDeltaMS});
                f += 1;
                t += config.qwDeltaMS;
                accumulator -= config.qwDeltaMS;
            }

            // Pass leftover time so we can do an interpolation if need be.
            m_pApp->Render(App::renderParams_s{accumulator, config.qwDeltaMS});
        }
    }

    // *************************************************************************

    auto Shutdown() -> void
    {
        m_pApp->Shutdown();
        GetPlatform().Shutdown();
    }
};

// *************************************************************************

static std::unique_ptr<Engine> g_pEngine;

// *************************************************************************

[[noreturn]] auto EngineMain(const args_t &nstrArgs) -> void
{
    if (g_cDefinedApp.get() == nullptr)
    {
        GetPlatform().FatalError("App must be defined, did you forget DEFINE_APPLICATION?");
    }

    g_pEngine.reset(new rock3d::Engine(std::move(g_cDefinedApp)));
    g_pEngine->Init(nstrArgs);

    for (;;)
    {
        g_pEngine->Tick();
    }
}

// *************************************************************************

[[noreturn]] auto Shutdown() -> void
{
    g_pEngine->Shutdown();
    exit(EXIT_SUCCESS);
}

// *************************************************************************

auto AppConfig() -> const App::config_s &
{
    return g_pEngine->AppConfig();
}

} // namespace rock3d
