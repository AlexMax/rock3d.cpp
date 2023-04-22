/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d
{

/**
 * @brief App interface - implemented by the game.
 */
class App
{
  public:
    struct config_s
    {
        /**
         * @brief Pretty name of the app.
         */
        const char *szName = nullptr;

        /**
         * @brief Number of MS in a single frame.
         */
        uint64_t qwDeltaMS = 0;

        /**
         * @brief Maximum number of MS a frame can take up.
         */
        uint64_t qwMaxMS = 0;
    };

    struct tickParams_s
    {
        uint64_t qwFrameCount = 0;
        uint64_t qwGameTime = 0;
        uint64_t qwDeltaTime = 0;
    };

    struct renderParams_s
    {
        uint64_t qwAccumTime = 0;
        uint64_t qwDeltaTime = 0;
    };

    ROCK3D_NOCOPY(App);

    App()
    {
    }

    virtual ~App()
    {
    }

    virtual auto Config() -> const config_s & = 0;
    virtual auto Init() -> void = 0;
    virtual auto HandleEvent(const rock3d::event_t &cEvent) -> void = 0;
    virtual auto Tick(const tickParams_s &cParams) -> void = 0;
    virtual auto Render(const renderParams_s &cParams) -> void = 0;
    virtual auto Shutdown() -> void = 0;
};

/**
 * @brief Entry point of the game engine, using the defined app.
 */
[[noreturn]] auto EngineMain() -> void;

/**
 * @brief Shutdown the game as gracefully as we can.
 */
[[noreturn]] auto Shutdown() -> void;

/**
 * @brief Return configuration values for the app.
 */
auto AppConfig() -> const App::config_s &;

} // namespace rock3d

/**
 * @brief Use this macro to define the application the engine should attach to.
 */
#define DEFINE_APPLICATION(cls) std::unique_ptr<rock3d::App> g_cDefinedApp{new cls()};
