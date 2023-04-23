/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Alex Mayfield <alexmax2742@gmail.com>
 */

/*
import { Atlas } from '../atlas';
import { AtlasTexture } from './atlasTexture';
import { Camera } from './camera';
import { DebugTextureContext } from './debug_texture';
import { WorldContext } from './worldContext';
import { WorldProgram } from './worldProgram';
import { FlatContext } from './flatContext';

const DEBUG: boolean = false;
*/

#include "../rock3d.h"

namespace rock3d::r3D
{

class RenderContext
{
    struct wallVert_s
    {
        glm::vec3 cPosition;
        glm::vec4 cAtlasInfo;
        glm::vec2 cTexCoord;
        glm::vec3 cBright;
    };
    std::vector<wallVert_s> m_ncWallVerts;

    std::unique_ptr<Textures> m_pTextures;

    bgfx::ProgramHandle m_cWorldShader = BGFX_INVALID_HANDLE;
    bgfx::VertexLayout m_cVertexLayout;
    bgfx::UniformHandle m_cUViewProj = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_cUTexure = BGFX_INVALID_HANDLE;

  public:
    auto Init() -> bool
    {
        m_cWorldShader = ShaderCompileProgram("rock3d/r3d/shaders/world");

        m_cVertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)  // Pos
            .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Float) // AtlasInfo
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float) // TexCoord
            .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)    // Bright
            .end();

        m_cUViewProj = bgfx::createUniform("u_viewProj", bgfx::UniformType::Mat4);
        m_cUTexure = bgfx::createUniform("u_texture", bgfx::UniformType::Sampler);

        return true;
    }

    /**
     * Persist the texture atlas onto the GPU.
     *
     * @param textures Texture atlas to bake.
     */
    auto BakeTextureAtlas() -> void
    {
    }

    /**
     * Add a wall to the set of things to render.
     *
     * Note that we need a working texture atlas at this point, otherwise
     * we have no clue what the texture coordinates need to be.
     *
     * @param cOne First vertex.
     * @param cTwo Second vertex.
     * @param fZ1 Floor height.
     * @param fZ2 Ceiling height.
     * @param strTexture Texture name.
     * @param cBright Wall brightness.
     */
    auto AddWall(const glm::vec2 &cOne, const glm::vec2 &cTwo, const float fZ1, const float fZ2,
                 const std::string_view strTexture, const glm::vec3 &cBright) -> bool
    {
        if (!m_pTextures)
        {
            return false;
        }

        // Find the texture of the wall in the atlas
        auto texEntry = m_pTextures->FindByName(strTexture);
        if (!texEntry)
        {
            return false;
        }

        const float ua1 = texEntry->cAtlasMin.x;
        const float va1 = texEntry->cAtlasMin.y;
        const float ua2 = texEntry->cAtlasMax.x;
        const float va2 = texEntry->cAtlasMax.y;

        const float hDist = glm::length(cTwo - cOne);
        const float vDist = fZ2 - fZ1;

        const float ut1 = 0.0f;
        const float vt1 = 0.0f;
        const float ut2 = hDist / texEntry->cPixelSize.x;
        const float vt2 = vDist / texEntry->cPixelSize.y;

        // Adjust brightness depending on which direction the wall is going.
        // Angles that are parallel to the X axis are darker, angles that
        // are parallel to the Y axis are brighter.  This is known as "fake
        // contrast".
        //
        // TODO: A triangle wave would probably look more consistent than
        //       a sinusoid, but this works well enough for now.
        const float angle = glm::atan(cTwo.y - cOne.y, cTwo.x - cOne.x);
        const float fBright = glm::cos(2 * angle) * -16;

        const float rBright = glm::clamp((cBright.r + fBright) / 256.0f, 0.0f, 1.0f);
        const float gBright = glm::clamp((cBright.g + fBright) / 256.0f, 0.0f, 1.0f);
        const float bBright = glm::clamp((cBright.b + fBright) / 256.0f, 0.0f, 1.0f);

        // Draw a wall into the vertex and index buffers.
        //
        // Assuming you want to face the square head-on, xyz1 is the lower-left
        // coordinate and xyz2 is the upper-right coordinate.
        m_ncWallVerts.push_back(wallVert_s{
            glm::vec3{cOne.x, cOne.y, fZ1},
            glm::vec4{ua1, va1, ua2 - ua1, va2 - va1},
            glm::vec2{ut1, vt2},
            glm::vec3{rBright, gBright, bBright},
        });
        m_ncWallVerts.push_back(wallVert_s{
            glm::vec3{cTwo.x, cTwo.y, fZ1},
            glm::vec4{ua1, va1, ua2 - ua1, va2 - va1},
            glm::vec2{ut2, vt2},
            glm::vec3{rBright, gBright, bBright},
        });
        m_ncWallVerts.push_back(wallVert_s{
            glm::vec3{cTwo.x, cTwo.y, fZ2},
            glm::vec4{ua1, va1, ua2 - ua1, va2 - va1},
            glm::vec2{ut2, vt1},
            glm::vec3{rBright, gBright, bBright},
        });
        m_ncWallVerts.push_back(wallVert_s{
            glm::vec3{cOne.x, cOne.y, fZ2},
            glm::vec4{ua1, va1, ua2 - ua1, va2 - va1},
            glm::vec2{ut1, vt1},
            glm::vec3{rBright, gBright, bBright},
        });

        /*
        this.worldInds[iCount] = vCount;
        this.worldInds[iCount + 1] = vCount + 1;
        this.worldInds[iCount + 2] = vCount + 2;
        this.worldInds[iCount + 3] = vCount + 2;
        this.worldInds[iCount + 4] = vCount + 3;
        this.worldInds[iCount + 5] = vCount + 0;
        */
    };
};

} // namespace rock3d::r3D
