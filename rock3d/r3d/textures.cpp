/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Alex Mayfield <alexmax2742@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "../rock3d.h"

#include "bimg/bimg.h"
#include "../vendor/stb_rect_pack.h"

namespace rock3d::r3D
{

class TexturesImpl final : public Textures
{
    //**************************************************************************

    static constexpr int ATLAS_SIZE = 2048;

    struct texture_s
    {
        texInfo_s cInfo;
        bimg::ImageContainer cImage;
    };

    std::vector<texture_s> m_ncTextures;
    std::unordered_map<std::string, size_t> m_cTextureNames;

    //**************************************************************************

  public:
    auto AddAsset(const std::string_view strAssetPath) -> bool override
    {
        auto maybeAsset = rock3d::GetAssets().ReadToBuffer(strAssetPath);
        if (!maybeAsset.has_value())
        {
            return false;
        }
        const rock3d::buffer_t &asset = maybeAsset.value();

        bimg::ImageContainer img;
        if (!bimg::imageParse(img, asset.data(), uint32_t(asset.size())))
        {
            return false;
        }

        // Add to internal tracking.
        const std::string path = std::string(strAssetPath);
        const size_t id = m_ncTextures.size();
        texture_s tex{texInfo_s{id, path}, img};
        m_ncTextures.push_back(tex);
        m_cTextureNames[path] = id;
        return true;
    }

    //**************************************************************************

    auto BakeAtlas() -> bool override
    {
        std::vector<stbrp_node> nodes;
        nodes.resize(size_t(ATLAS_SIZE));
        stbrp_context ctx;
        stbrp_init_target(&ctx, ATLAS_SIZE, ATLAS_SIZE, nodes.data(), nodes.size());

        // Set up a rects array so we have a place to write our coordinates.
        std::vector<stbrp_rect> rects;
        for (auto &tex : m_ncTextures)
        {
            stbrp_rect rect;
            rect.id = int(tex.cInfo.qwID);
            rect.w = stbrp_coord(tex.cImage.m_width);
            rect.h = stbrp_coord(tex.cImage.m_height);
        }

        // Do the packing.
        if (!stbrp_pack_rects(&ctx, rects.data(), rects.size()))
        {
            return false;
        }

        // Update the atlas position of each texture.
        for (auto &rect : rects)
        {
            // Calculate atlas coordinates.
            auto &tex = m_ncTextures[size_t(rect.id)];
            tex.cInfo.cPixelSize = {rect.x, rect.y};
            tex.cInfo.cAtlasMin = {
                float(rect.x) / ATLAS_SIZE,
                float(rect.y) / ATLAS_SIZE,
            };
            tex.cInfo.cAtlasMax = {
                float(rect.x + rect.w) / ATLAS_SIZE,
                float(rect.y + rect.h) / ATLAS_SIZE,
            };
        }
    }

    //**************************************************************************

    auto ToGPU() -> void override
    {
    }

    //**************************************************************************

    auto FindByID(const size_t qwID) -> const texInfo_s * override
    {
        if (qwID >= m_ncTextures.size())
        {
            return nullptr;
        }
        return &m_ncTextures[qwID].cInfo;
    }

    //**************************************************************************

    auto FindByName(const std::string_view strAssetPath) -> const texInfo_s * override
    {
        auto it = m_cTextureNames.find(std::string(strAssetPath));
        if (it == m_cTextureNames.end())
        {
            return nullptr;
        }
        return &m_ncTextures[it->second].cInfo;
    }
};

//******************************************************************************

auto Textures::Alloc() -> std::unique_ptr<Textures>
{
    return std::unique_ptr<Textures>(new TexturesImpl());
}

} // namespace rock3d::r3D
