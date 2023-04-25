/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Alex Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d::r3D
{

class Textures
{
  public:
    struct texInfo_s
    {
        size_t qwID = 0;
        std::string strName;
        glm::ivec2 cPixelSize;
        glm::vec2 cAtlasMin;
        glm::vec2 cAtlasMax;
    };

    virtual auto AddAsset(const std::string_view strAssetPath) -> bool = 0;
    virtual auto BakeAtlas() -> bool = 0;
    virtual auto ToGPU() -> void = 0;
    virtual auto FindByID(const size_t qwID) -> const texInfo_s * = 0;
    virtual auto FindByName(const std::string_view strAssetPath) -> const texInfo_s * = 0;

    static auto Alloc() -> std::unique_ptr<Textures>;
};

} // namespace rock3d::r3D
