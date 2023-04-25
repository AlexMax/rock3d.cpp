/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d
{

/**
 * Polar coordinates as radius, angle (in radians) pair.
 */
using Polar = glm::vec2;

inline auto Hypot(glm::vec2 v) -> float
{
    return std::hypotf(v.x, v.y);
}

} // namespace rock3d
