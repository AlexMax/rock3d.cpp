/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d
{

struct Edge
{
    /**
     * First vertex of the edge.
     */
    glm::vec2 cVertex;

    /**
     * Second vertex of the edge, as a reference.
     */
    glm::vec2 cNextVertex;

    /**
     * Upper texture.
     *
     * Used on edge with a backside to texture the wall above the "portal".
     */
    std::string strUpperTex;

    /**
     * Middle texture.
     *
     * Used on normal walls with no backside as their primary wall texture
     * or sides with a backside when you want a texture covering the "portal".
     */
    std::string strMiddleTex;

    /**
     * Lower texture.
     *
     * Used on sides with a backside to texture the wall below the "portal".
     */
    std::string strLowerTex;

    /**
     * Polygon ID of the polygon on the opposite side of this side.
     *
     * Used if this side should be a portal to another polygon, or null if
     * the polygon should just be a wall.
     */
    size_t qwBackPoly;

    /**
     * Normal vector of Edge.
     *
     * Calculated at runtime.  Must be refreshed if current or next vertex
     * is moved.
     */
    glm::vec2 cNormalCache;
};

struct Polygon
{
    /**
     * Edges of polygon, as ID's.
     */
    std::vector<size_t> nqwEdgeIDs;

    /**
     * Floor height.
     */
    float fFloorHeight;

    /**
     * Ceiling height.
     */
    float fCeilHeight;

    /**
     * Floor texture.
     */
    std::string strFloorTex;

    /**
     * Ceiling texture.
     */
    std::string strCeilTex;

    /**
     * Sector brightness.
     */
    glm::vec3 cBrightness;

    /**
     * Tessellation vertex cache.
     */
    std::vector<glm::vec2> nqwVertsCache;

    /**
     * Tessellation index cache for floor.
     */
    std::vector<size_t> nqwFloorIndsCache;

    /**
     * Tessellation index cache for ceiling.
     */
    std::vector<size_t> nqwCeilIndsCache;
};

struct Location
{
    /**
     * Type of location, as a string.
     */
    std::string strType;

    /**
     * Entity config of spawners.
     */
    std::string strEntityConfig;

    /**
     * Polygon that the location is located inside.
     */
    size_t qwPolygon;

    /**
     * Position of the location.
     */
    glm::vec3 cPosition;

    /**
     * Rotation of the location.
     */
    glm::quat cRotation;
};

struct Level
{
    /**
     * All polygons in the level data.
     */
    std::vector<Polygon> ncPolygons;

    /**
     * All edges in the level data.
     */
    std::vector<Edge> ncEdges;

    /**
     * All locations in the level data.
     */
    std::vector<Location> ncLocations;
};

enum class loadLevelError_e
{
    missing_asset,
    json_parse_error,
};

using loadLevelResult_t = nonstd::expected<Level, loadLevelError_e>;

/**
 * @brief Load a level from an asset path.
 *
 * @param strPath Asset filepath.
 * @return Constructed level, or error.
 */
auto LoadLevelAsset(const std::string_view strPath) -> loadLevelResult_t;

} // namespace rock3d
