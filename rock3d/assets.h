/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d
{

class Assets
{
  public:
    virtual ~Assets()
    {
    }

    enum class readError_t
    {
        invalid_path,
        not_found,
    };
    using readResult_t = nonstd::expected<buffer_t, readError_t>;

    virtual auto AddPath(std::string_view strPath) -> void = 0;
    virtual auto ReadToBuffer(std::string_view strPath) -> readResult_t = 0;
};

auto GetAssets() -> Assets &;

} // namespace rock3d
