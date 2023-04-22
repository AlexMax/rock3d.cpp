/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#include "rock3d.h"

namespace rock3d::Random
{

//******************************************************************************

static Xoshiro256pp g_cRandom;

//******************************************************************************

auto SetEntropySeed(Xoshiro256pp &cRNG) -> void
{
    std::random_device rd;
    cRNG.seed(rd(), rd());
}

//******************************************************************************

auto SetEntropySeed() -> void
{
    SetEntropySeed(g_cRandom);
}

//******************************************************************************

auto SetSeed(Xoshiro256pp &cRNG, const uint64_t qwSeed) -> void
{
    cRNG.seed(qwSeed);
}

//******************************************************************************

auto SetSeed(const uint64_t qwSeed) -> void
{
    SetSeed(g_cRandom, qwSeed);
}

//******************************************************************************

auto U64(Xoshiro256pp &cRNG) -> uint64_t
{
    return cRNG();
}

//******************************************************************************

auto U64() -> uint64_t
{
    return U64(g_cRandom);
}

//******************************************************************************

auto UniformU64(Xoshiro256pp &cRNG, const uint64_t qwUpperBound) -> uint64_t
{
    if (qwUpperBound < 2)
    {
        return 0;
    }

    std::uniform_int_distribution<uint64_t> dist(0, qwUpperBound - 1);
    return dist(cRNG);
}

//******************************************************************************

auto UniformU64(const uint64_t qwUpperBound) -> uint64_t
{
    return UniformU64(g_cRandom, qwUpperBound);
}

//******************************************************************************

auto Float(Xoshiro256pp &cRNG) -> float
{
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(cRNG);
}

//******************************************************************************

auto Float() -> float
{
    return Float(g_cRandom);
}

} // namespace rock3d::Random
