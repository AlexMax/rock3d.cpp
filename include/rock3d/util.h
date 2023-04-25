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

template <class DST, class SRC>
auto BitCast(const SRC &src) noexcept
{
    static_assert(sizeof(DST) == sizeof(SRC), "types must be the same size");
    static_assert(std::is_trivially_copyable_v<SRC>, "source type must be trivially copyable");
    static_assert(std::is_trivially_copyable_v<DST>, "dest type must be trivially copyable");
    static_assert(std::is_trivially_constructible_v<DST>, "dest type must be trivially constructable");
    DST dst;
    std::memcpy(&dst, &src, sizeof(SRC));
    return dst;
}

} // namespace rock3d
