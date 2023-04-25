/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

/**
 * @brief This is a platform implementation for Win32, which utilizes SDL
 *        for many pieces for functionality.
 */

#include "rock3d/rock3d.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stringapiset.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include "bgfx/platform.h"

namespace rock3d
{

enum class encodeError_e
{
    invalid_unicode, // Invalid unicode was pseed to conversion function.
    internal_error,  // A problem with the conversion routine itself.
};

/**
 * @brief Convert a UTF8 string to a WCHAR-compatible wstring.
 *
 * @detail There should
 *
 * @param strInput UTF8-encoded to convert.
 * @return A converted wstring which can be passed to Wide Win32 functions.
 */
static auto UTF8ToWString(const std::string_view strInput) -> nonstd::expected<std::wstring, encodeError_e>
{
    if (strInput.empty())
    {
        // Empty strings don't get converted - since the conversion function
        // returns 0 for errors, we return early here.
        return std::wstring();
    }

    const int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, strInput.data(), strInput.size(), nullptr, 0);
    if (length == 0)
    {
        DWORD err = GetLastError();
        if (err == ERROR_NO_UNICODE_TRANSLATION)
        {
            return nonstd::make_unexpected(encodeError_e::invalid_unicode);
        }
        return nonstd::make_unexpected(encodeError_e::internal_error);
    }

    std::wstring rvo;
    rvo.resize(length); // Don't make room for a null terminator.

    const int ok =
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, strInput.data(), strInput.size(), rvo.data(), rvo.size());
    if (ok == 0)
    {
        return nonstd::make_unexpected(encodeError_e::internal_error);
    }

    return rvo;
}

//******************************************************************************

class Win32Platform final : public Platform
{
    constexpr static int DEFAULT_SCREEN_WIDTH = 1280;
    constexpr static int DEFAULT_SCREEN_HEIGHT = 720;

    SDL_Window *m_pWindow = nullptr;
    SDL_Surface *m_pSurface = nullptr;
    HWND handle = nullptr;

  public:
    auto Init() -> void override
    {
        // Initialize SDL.
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            GetPlatform().FatalError(SDL_GetError());
            return;
        }

        m_pWindow = SDL_CreateWindow(AppConfig().szName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                     DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (m_pWindow == nullptr)
        {
            GetPlatform().FatalError(SDL_GetError());
            return;
        }

        m_pSurface = SDL_GetWindowSurface(m_pWindow);
        if (m_pSurface == nullptr)
        {
            GetPlatform().FatalError(SDL_GetError());
            return;
        }

        // Attach bgfx to SDL window.
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(m_pWindow, &wmi))
        {
            GetPlatform().FatalError(SDL_GetError());
            return;
        }
        glm::ivec2 res = WindowResolution();

        // Initialize bgfx.
        bgfx::PlatformData pd{};
        pd.nwh = wmi.info.win.window;
        bgfx::setPlatformData(pd);
        auto render = bgfx::renderFrame();

        bgfx::Init bgfx_init;
        bgfx_init.type = bgfx::RendererType::Vulkan;
        bgfx_init.resolution.width = res.x;
        bgfx_init.resolution.height = res.y;
        bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx_init.platformData = pd;
        bgfx::init(bgfx_init);

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x007ffffff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, res.x, res.y);
    }

    //**************************************************************************

    auto Shutdown() -> void override
    {
        if (!m_pWindow)
        {
            SDL_DestroyWindow(m_pWindow);
        }

        SDL_Quit();
    }

    //**************************************************************************

    auto TimeMS() -> uint64_t override
    {
        return SDL_GetTicks64();
    }

    //**************************************************************************

    auto SleepMS(const uint32_t dwMillis) -> void override
    {
        return SDL_Delay(dwMillis);
    }

    //**************************************************************************

    auto WindowHandle() -> void * override
    {
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        if (SDL_GetWindowWMInfo(m_pWindow, &info))
        {
            return (void *)info.info.win.window;
        }
        else
        {
            return nullptr;
        }
    }

    //**************************************************************************

    auto WindowResolution() -> glm::ivec2 override
    {
        glm::ivec2 rvo;
        SDL_GetWindowSize(m_pWindow, &rvo.x, &rvo.y);
        return rvo;
    }

    //**************************************************************************

    auto GetBasePath() -> std::string_view override
    {
        static std::string basePath;
        if (basePath.empty())
        {
            basePath = SDL_GetBasePath();
        }
        return basePath;
    }

    //**************************************************************************

    [[noreturn]] auto FatalError(const std::string_view strError) -> void override
    {
        int result = 0;
        auto maybeError = UTF8ToWString(strError);
        if (!maybeError.has_value())
        {
            result = MessageBoxA(nullptr, std::string(strError).c_str(), "rock3d Error (Retry to Debug)",
                                 MB_RETRYCANCEL | MB_ICONERROR);
        }
        else
        {
            result = MessageBoxW(nullptr, maybeError.value().c_str(), L"rock3d Error (Retry to Debug)",
                                 MB_RETRYCANCEL | MB_ICONERROR);
        }
        if (result == IDRETRY)
        {
            __debugbreak();
        }

        exit(EXIT_FAILURE);
    }

    //**************************************************************************

    auto ReadFileToBuffer(const std::string_view strFilePath) -> readResult_t override
    {
        auto maybeFilePath = UTF8ToWString(strFilePath);
        if (!maybeFilePath.has_value())
        {
            return nonstd::make_unexpected(readError_e::invalid_path);
        }

        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        const std::wstring filePath = maybeFilePath.value();
        const HANDLE fh = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL, nullptr);
        if (fh == INVALID_HANDLE_VALUE)
        {
            return nonstd::make_unexpected(readError_e::file_not_found);
        }

        buffer_t rvo;
        std::array<uint8_t, 8192> buffer;
        for (;;)
        {
            DWORD bytesRead = 0;
            BOOL ok = ReadFile(fh, buffer.data(), buffer.size(), &bytesRead, nullptr);
            if (!ok)
            {
                return nonstd::make_unexpected(readError_e::file_read_error);
            }
            else if (bytesRead == 0)
            {
                return rvo;
            }
            rvo.reserve(rvo.size() + bytesRead);
            std::copy(buffer.begin(), buffer.begin() + bytesRead, std::back_inserter(rvo));
        }
    };

    /**
     * @brief Convert an SDL scancode to our keyboardScan_e enum.
     */
    static auto SDLScanToRock(const SDL_Scancode eScan) -> keyboardScan_e
    {
        return keyboardScan_e(eScan);
    }

    /**
     * @brief Convert an SDL scancode to our keyboardScan_e enum.
     */
    static auto SDLKeyToRock(const SDL_Keycode eKey) -> keyboardKey_e
    {
        return keyboardKey_e(eKey);
    }

    /**
     * @brief Convert a SDL mouse button to our mouseButton_e enum.
     */
    static auto SDLMBtnToRock(const Uint8 button) -> mouseButton_e
    {
        switch (button)
        {
        case SDL_BUTTON_LEFT:
            return MBTN_LEFT;
        case SDL_BUTTON_MIDDLE:
            return MBTN_MIDDLE; // Unlike SDL, we use 3 for middle.
        case SDL_BUTTON_RIGHT:
            return MBTN_RIGHT; // Unlike SDL, we use 2 for right.
        case SDL_BUTTON_X1:
            return MBTN_X1;
        case SDL_BUTTON_X2:
            return MBTN_X2;
        }
        return MBTN_NONE;
    }

    /**
     * @brief Convert an SDL gamepad axis to our padAxis_e enum.
     */
    static auto SDLPadAxisToRock(const SDL_GameControllerAxis eAxis) -> padAxis_e
    {
        return padAxis_e(eAxis);
    }

    /**
     * @brief Convert an SDL gamepad button to our padButton_e enum.
     */
    static auto SDLPadButtonToRock(const SDL_GameControllerButton eButton) -> padButton_e
    {
        return padButton_e(eButton);
    }

    //**************************************************************************

    virtual auto PumpEvents() -> void
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_KEYDOWN:
                [[fallthrough]];
            case SDL_KEYUP: {
                event_t rockEv = eventKey_s{
                    SDLScanToRock(ev.key.keysym.scancode),
                    SDLKeyToRock(ev.key.keysym.sym),
                    ev.type == SDL_KEYDOWN,
                };
                GetEventQueue().Queue(std::move(rockEv));
                break;
            }
            case SDL_MOUSEMOTION: {
                event_t rockEv = eventMouseMotion_s{
                    glm::ivec2{ev.motion.x, ev.motion.y},
                    glm::ivec2{ev.motion.xrel, ev.motion.yrel},
                };
                GetEventQueue().Queue(std::move(rockEv));
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
                [[fallthrough]];
            case SDL_MOUSEBUTTONUP: {
                event_t rockEv = eventMouseButton_s{
                    glm::ivec2{ev.button.x, ev.button.y},
                    SDLMBtnToRock(ev.button.button),
                    ev.button.state == SDL_PRESSED,
                };
                GetEventQueue().Queue(std::move(rockEv));
                break;
            }
            case SDL_MOUSEWHEEL:
                break;
            case SDL_CONTROLLERAXISMOTION: {
                event_t rockEv = eventPadAxis_s{ev.caxis.axis, ev.caxis.value};
                GetEventQueue().Queue(std::move(rockEv));
                break;
            }
            case SDL_CONTROLLERBUTTONDOWN:
                [[fallthrough]];
            case SDL_CONTROLLERBUTTONUP: {
                event_t rockEv = eventPadButton_s{ev.cbutton.button, ev.type == SDL_CONTROLLERBUTTONDOWN};
                GetEventQueue().Queue(std::move(rockEv));
                break;
            }
            default:
                break;
            }
        }
    }
};

//******************************************************************************

auto GetPlatform() -> Platform &
{
    static Win32Platform platform;
    return platform;
}

} // namespace rock3d

//******************************************************************************

auto main(int argc, char *argv[]) -> int
{
    rock3d::EngineMain();
    return EXIT_FAILURE;
}
