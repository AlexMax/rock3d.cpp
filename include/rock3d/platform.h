/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

/*
 * Platform abstraction used by all platforms.
 *
 * - Encoding of files and paths is assumed to be in UTF-8, with forward
 *   slashes as a path separator.
 */

#pragma once

namespace rock3d
{

class Platform
{
  public:
    Platform() {}
    virtual ~Platform() {}
    ROCK3D_NOCOPY(Platform);

    enum class readError_e
    {
        invalid_path,    // Path could not be constructed.
        file_not_found,  // Path did not lead to a valid file.
        file_read_error, // File could not be fully read.
    };

    using readResult_t = nonstd::expected<buffer_t, readError_e>;

    /**
     * @brief Initialize platform.
     */
    virtual auto Init() -> void = 0;

    /**
     * @brief Shutdown platform.
     */
    virtual auto Shutdown() -> void = 0;

    /**
     * @brief A monotonic timer with millisecond accuracy.
     */
    virtual auto TimeMS() -> uint64_t = 0;

    /**
     * @brief Sleep this thread for no less than the passed number of
     *        milliseconds.
     * @param dwMillis Number of milliseconds to sleep for.
     */
    virtual auto SleepMS(const uint32_t dwMillis) -> void = 0;

    /**
     * @brief Get a handle to the main window.
     */
    virtual auto WindowHandle() -> void * = 0;

    /**
     * @brief Return the current resolution of the main window.
     */
    virtual auto WindowResolution() -> glm::ivec2 = 0;

    /**
     * @brief Get base path where executable is located.
     */
    virtual auto GetBasePath() -> std::string_view = 0;

    /**
     * @brief Show a fatal error message, then quit.
     *
     * @param err Error message to show.
     */
    virtual auto FatalError(const std::string_view strError) -> void = 0;

    /**
     * @brief Read the contents of a file into a buffer.
     *
     * @param strFilePath File to read.
     * @return A buffer containing the file's data, or an error on failure.
     */
    virtual auto ReadFileToBuffer(const std::string_view strFilePath) -> readResult_t = 0;

    /**
     * @brief Pump events into a form that we can use later.
     */
    virtual auto PumpEvents() -> void = 0;
};

auto GetPlatform() -> Platform &;

} // namespace rock3d
