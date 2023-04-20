/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d
{

static auto Rotl(const uint64_t x, int k) -> uint64_t
{
    return (x << k) | (x >> (64 - k));
}

} // namespace rock3d
