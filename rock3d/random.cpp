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

auto U32(Xoshiro256pp &cRNG) -> uint32_t
{
    return uint32_t(cRNG());
}

//******************************************************************************

auto U32() -> uint32_t
{
    return U32(g_cRandom);
}

//******************************************************************************

auto UniformU32(Xoshiro256pp &cRNG, const uint32_t dwUpperBound) -> uint32_t
{
    // Specific implementations of <random> are not guaranteed to be
    // consistent, so we roll our own uniform distribution.
    //
    // Uniform distribution with integers is easy - if you generate a number
    // that falls outside a range that would be uniform, discard your number
    // and reroll.

    if (dwUpperBound < 2)
    {
        return 0;
    }

    const uint32_t min = (0 - dwUpperBound) % dwUpperBound;

    uint32_t res = 0;
    for (;;)
    {
        res = U64();
        if (res >= min)
        {
            return res % dwUpperBound;
        }
    }
}

//******************************************************************************

auto UniformU32(const uint32_t qwUpperBound) -> uint32_t
{
    return UniformU32(g_cRandom, qwUpperBound);
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
    // Specific implementations of <random> are not guaranteed to be
    // consistent, so we roll our own uniform distribution.
    //
    // Uniform distribution with integers is easy - if you generate a number
    // that falls outside a range that would be uniform, discard your number
    // and reroll.

    if (qwUpperBound < 2)
    {
        return 0;
    }

    const uint64_t min = (0 - qwUpperBound) % qwUpperBound;

    uint64_t res = 0;
    for (;;)
    {
        res = U64();
        if (res >= min)
        {
            return res % qwUpperBound;
        }
    }
}

//******************************************************************************

auto UniformU64(const uint64_t qwUpperBound) -> uint64_t
{
    return UniformU64(g_cRandom, qwUpperBound);
}

//******************************************************************************

auto Float(Xoshiro256pp &cRNG) -> float
{
    // Specific implementations of <random> are not guaranteed to be
    // consistent, so we roll our own uniform distribution.
    //
    // Uniform distribution with floats is tricky because of precision
    // differences in different number ranges.  The distribution inside
    // [1.0, 2.0) is consistent, so we generate an integer that fits in
    // the binary representation

    constexpr uint32_t RANDOM_MASK = 0x7fffff;
    constexpr uint32_t RANDOM_EXP = 0x3f800000;
    const uint32_t rand = (U32() & RANDOM_MASK) | RANDOM_EXP;
    return BitCast<float>(rand) - 1.0f;
}

//******************************************************************************

auto Float() -> float
{
    return Float(g_cRandom);
}

} // namespace rock3d::Random
