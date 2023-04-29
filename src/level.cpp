/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#include "rock3d/rock3d.h"

#include "json/json.h"
#include "./vendor/mapbox/earcut.hpp"

namespace rock3d
{

/**
 * @brief Unpack an edge JSON.
 */
static auto UnserializeEdge(const Json::Value &cJson) -> Edge
{
    Edge edge;
    edge.cVertex.x = cJson["vertex"][0].asFloat();
    edge.cVertex.y = cJson["vertex"][1].asFloat();
    edge.strUpperTex = cJson["upperTex"].asString();
    edge.strMiddleTex = cJson["middleTex"].asString();
    edge.strLowerTex = cJson["lowerTex"].asString();
    if (cJson.isMember("backPoly"))
    {
        edge.qwBackPoly = cJson["backPoly"].asUInt64();
    }
    else
    {
        edge.qwBackPoly = UINT64_MAX;
    }
    return edge;
}

/**
 * @brief Unpack a polygon JSON.
 */
static auto UnserializePolygon(const Json::Value &cJson, Level &cLevel) -> Polygon
{
    // Unpack all the edges.
    Polygon polygon;
    for (auto &jsonEdge : cJson["edges"])
    {
        polygon.nqwEdgeIDs.push_back(cLevel.ncEdges.size());
        Edge edge = UnserializeEdge(jsonEdge);
        cLevel.ncEdges.push_back(edge);
    }

    // Cache the next vertex in each edge of the polygon.
    const size_t edgeCount = polygon.nqwEdgeIDs.size();
    for (size_t i = 0; i < edgeCount; i++)
    {
        Edge &curEdge = cLevel.ncEdges[i];
        curEdge.cNextVertex = cLevel.ncEdges[(i + 1) % edgeCount].cVertex;
    }

    polygon.fFloorHeight = cJson["floorHeight"].asFloat();
    polygon.fCeilHeight = cJson["ceilHeight"].asFloat();
    polygon.strFloorTex = cJson["floorTex"].asString();
    polygon.strCeilTex = cJson["ceilTex"].asString();
    polygon.cBrightness.r = cJson["brightness"][0].asUInt() / 256.0f;
    polygon.cBrightness.g = cJson["brightness"][1].asUInt() / 256.0f;
    polygon.cBrightness.b = cJson["brightness"][2].asUInt() / 256.0f;
    return polygon;
}

/**
 * @brief Unpack a location JSON.
 */
static auto UnserializeLocation(const Json::Value &cJson) -> Location
{
    Location location;
    location.strType = cJson["type"].asString();
    location.strEntityConfig = cJson["entityConfig"].asString();
    location.qwPolygon = cJson["polygon"].asUInt64();
    location.cPosition[0] = cJson["position"][0].asFloat();
    location.cPosition[1] = cJson["position"][1].asFloat();
    location.cPosition[2] = cJson["position"][2].asFloat();

    glm::vec3 euler;
    euler.x = cJson["rotation"][0].asFloat(); // pitch
    euler.y = cJson["rotation"][1].asFloat(); // yaw
    euler.z = cJson["rotation"][2].asFloat(); // roll
    location.cRotation = glm::quat(euler);
    return location;
}

/**
 * @brief Unpack the level JSON.
 */
auto UnserializeLevel(const Json::Value &cJson) -> Level
{
    Level level;
    for (auto &jsonPolygon : cJson["polygons"])
    {
        level.ncPolygons.push_back(UnserializePolygon(jsonPolygon, level));
    }
    for (auto &jsonLocation : cJson["locations"])
    {
        level.ncLocations.push_back(UnserializeLocation(jsonLocation));
    }
    return level;
}

/**
 * @brief Tessellate the floor and ceiling.
 */
static auto CacheTessellation(Polygon &cOutPoly, const std::vector<Edge> &ncEdges) -> void
{
    using earPoint_t = std::array<float, 2>;

    // Create shape.
    std::vector<glm::vec2> verts;
    std::vector<earPoint_t> shape;
    for (size_t i = 0; i < cOutPoly.nqwEdgeIDs.size(); i++)
    {
        const auto &vert = ncEdges[cOutPoly.nqwEdgeIDs[i]].cVertex;
        verts.push_back(vert);
        shape.push_back(earPoint_t{vert.x, vert.y});
    }

    // Add shape to polygon - no holes.
    std::array<std::vector<earPoint_t>, 1> polygon = {shape};

    // Do the tessellation.
    cOutPoly.nqwVertsCache = verts;
    cOutPoly.nqwFloorIndsCache = mapbox::earcut<size_t>(polygon);
    cOutPoly.nqwCeilIndsCache = cOutPoly.nqwFloorIndsCache;
    std::reverse(cOutPoly.nqwCeilIndsCache.begin(), cOutPoly.nqwCeilIndsCache.end());
}

/**
 * @brief Cache the normal vector for an edge.
 */
static auto CacheNormal(Edge &cOutEdge) -> void
{
    const auto &frontOne = cOutEdge.cVertex;
    const auto &frontTwo = cOutEdge.cNextVertex;

    cOutEdge.cNormalCache = glm::vec2{frontTwo[1] - frontOne[1], -(frontTwo[0] - frontOne[0])};
}

// *****************************************************************************

auto LoadLevelAsset(const std::string_view strPath) -> loadLevelResult_t
{
    // Find the level file.
    auto result = GetAssets().ReadToBuffer(strPath);
    if (!result.has_value())
    {
        return nonstd::make_unexpected(loadLevelError_e::missing_asset);
    }

    // Parse the JSON.
    buffer_t &data = result.value();
    Json::Value root;
    Json::Reader reader;
    const char *start = (const char *)&data[0];
    const char *end = start + data.size();
    const int ok = reader.parse(start, end, root);
    if (!ok)
    {
        return nonstd::make_unexpected(loadLevelError_e::json_parse_error);
    }

    // Unserialize the level.
    Level level = UnserializeLevel(root);

    // Cache polygon tessellation.
    for (auto &poly : level.ncPolygons)
    {
        CacheTessellation(poly, level.ncEdges);
    }

    // Cache edge normal vector.
    for (auto &edge : level.ncEdges)
    {
        CacheNormal(edge);
    }

    return level;
}

// *****************************************************************************

}; // namespace rock3d
