/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

namespace rock3d
{

/**
 * @brief This is xoshiro256++ 1.0, one of our all-purpose, rock-solid
 *        generators.  It has excellent (sub-ns) speed, a state (256 bits)
 *        that is large enough for any parallel application, and it passes
 *        all tests we are aware of.
 *
 *        The state must be seeded so that it is not everywhere zero.
 */
class Xoshiro256pp
{
    std::array<uint64_t, 4> m_nqwState;

    auto SplitMix64(uint64_t x) -> uint64_t
    {
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

  public:
    using result_type = uint64_t;

    Xoshiro256pp(const uint32_t seed0, const uint32_t seed1)
    {
        m_nqwState[0] = uint64_t(seed0) | (uint64_t(seed1) << 32);
        m_nqwState[1] = SplitMix64(m_nqwState[0]);
        m_nqwState[2] = SplitMix64(m_nqwState[1]);
        m_nqwState[3] = SplitMix64(m_nqwState[2]);
    }

    auto operator()() -> result_type
    {
        const uint64_t result = Rotl(m_nqwState[0] + m_nqwState[3], 23) + m_nqwState[0];

        const uint64_t t = m_nqwState[1] << 17;

        m_nqwState[2] ^= m_nqwState[0];
        m_nqwState[3] ^= m_nqwState[1];
        m_nqwState[1] ^= m_nqwState[2];
        m_nqwState[0] ^= m_nqwState[3];

        m_nqwState[2] ^= t;

        m_nqwState[3] = Rotl(m_nqwState[3], 45);

        return result;
    }

    static constexpr auto min() -> result_type
    {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr auto max() -> result_type
    {
        return std::numeric_limits<result_type>::max();
    }
};

} // namespace rock3d
