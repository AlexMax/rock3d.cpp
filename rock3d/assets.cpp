/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#include "rock3d.h"

namespace rock3d
{

/**
 * @brief Check to see if the caller is trying to escape the passed directory.
 *
 * @param strPath Path to check.
 * @return True if a parent directory escape was found.
 */
static bool ContainsParentDir(const std::string_view strPath)
{
    if (strPath.find("../") != std::string_view::npos)
    {
        // Parent directory with a trailing directory.
        return true;
    }
    else if (strPath.find("..\\") != std::string_view::npos)
    {
        // Parent directory with a trailing directory, Win32-style.
        return true;
    }
    if (strPath.length() > 1)
    {
        // Path ends with a .. - only a problem if it's the only string,
        // but better safe than sorry.
        return strPath.compare(strPath.length() - 2, 2, "..") == 0;
    }
    return false;
}

class AssetsImpl final : public Assets
{
    struct resLoc_s
    {
        std::string location;
    };
    std::vector<resLoc_s> m_ncResLocs;

  public:
    auto AddPath(const std::string_view strPath) -> void override
    {
        m_ncResLocs.push_back(resLoc_s{std::string{strPath}});
    }

    auto ReadToBuffer(const std::string_view strPath) -> readResult_t override
    {
        if (ContainsParentDir(strPath))
        {
            return nonstd::make_unexpected(readError_t::invalid_path);
        }
        for (auto &resloc : m_ncResLocs)
        {
            const std::string fullPath = fmt::format("{}/{}", resloc.location, strPath);
            auto maybeFile = GetPlatform().ReadFileToBuffer(fullPath);
            if (maybeFile.has_value())
            {
                return maybeFile.value();
            }
        }
        return nonstd::make_unexpected(readError_t::not_found);
    }
};

auto GetAssets() -> Assets &
{
    static AssetsImpl assets;
    return assets;
}

} // namespace rock3d
