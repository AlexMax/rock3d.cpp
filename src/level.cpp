/*
 * rock3d.js: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#include "rock3d/rock3d.h"

#include "json/json.h"

namespace rock3d
{

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

static auto UnserializePolygon(const Json::Value &cJson, Level &cLevel) -> Polygon
{
    Polygon polygon;
    for (auto &jsonEdge : cJson["edges"])
    {
        polygon.nqwEdgeIDs.push_back(cLevel.ncEdges.size());
        Edge edge = UnserializeEdge(jsonEdge);
        cLevel.ncEdges.push_back(edge);
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

auto LoadLevelAsset(const std::string_view strPath) -> loadLevelResult_t
{
    auto result = GetAssets().ReadToBuffer(strPath);
    if (!result.has_value())
    {
        return nonstd::make_unexpected(loadLevelError_e::missing_asset);
    }

    buffer_t &data = result.value();
    Json::Value root;
    Json::Reader reader;
    const int ok = reader.parse((const char *)&data[0], (const char *)&data[data.size()], root);
    if (!ok)
    {
        return nonstd::make_unexpected(loadLevelError_e::json_parse_error);
    }

    return UnserializeLevel(root);
}

}; // namespace rock3d
