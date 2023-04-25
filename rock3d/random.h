/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

namespace rock3d::Random
{

/**
 * @brief This is xoshiro256++ 1.0, one of our all-purpose, rock-solid
 *        generators.  It has excellent (sub-ns) speed, a state (256 bits)
 *        that is large enough for any parallel application, and it passes
 *        all tests we are aware of.
 *
 *        The state must be seeded so that it is not everywhere zero.
 *
 * @date 2019
 * @authors David Blackman and Sebastiano Vigna (vigna@acm.org)
 * @copyright http://creativecommons.org/publicdomain/zero/1.0/
 */
class Xoshiro256pp
{
    std::array<uint64_t, 4> m_nqwState;

    /**
     * @brief This is a fixed-increment version of Java 8's SplittableRandom
     *        generator
     *
     *        It is a very fast generator passing BigCrush, and it can be
     *        useful if for some reason you absolutely want 64 bits of state.
     *
     * @date 2015
     * @authors David Blackman and Sebastiano Vigna (vigna@acm.org)
     * @copyright http://creativecommons.org/publicdomain/zero/1.0/
     *
     * @param qwState Initial 64-bit state.
     * @return Return a random 64-bit integer.
     */
    auto SplitMix64(uint64_t qwState) -> uint64_t
    {
        uint64_t z = (qwState += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

  public:
    using result_type = uint64_t;

    /**
     * @brief The default seed used when no other seed has been passed.
     *
     * @note Blackman and Vigna do not specify a de-facto default seed.
     */
    static constexpr uint64_t default_seed = 19931210;

    Xoshiro256pp()
    {
        seed(default_seed);
    }

    Xoshiro256pp(const uint32_t dwInit0, const uint32_t dwInit1)
    {
        seed(dwInit0, dwInit1);
    }

    Xoshiro256pp(const uint64_t initSeed)
    {
        seed(initSeed);
    }

    /**
     * @brief Set internal state of the generator using a 64-bit seed.
     *
     * @details Based on advice from Blackman and Vigna:
     *
     *          We suggest to use SplitMix64 to initialize the state of our
     *          generators starting from a 64-bit seed, as research has shown
     *          that initialization must be performed with a generator radically
     *          different in nature from the one initialized to avoid
     *          correlation on similar seeds.
     */
    auto seed(const uint64_t qwSeed) -> void
    {
        m_nqwState[0] = SplitMix64(qwSeed);
        m_nqwState[1] = SplitMix64(m_nqwState[0]);
        m_nqwState[2] = SplitMix64(m_nqwState[1]);
        m_nqwState[3] = SplitMix64(m_nqwState[2]);
    }

    /**
     * @brief Seed the generator using two 32-bit seeds.
     *
     * @details This exists so it can be called with the output of
     *          std::random_device.  Otherwise, use a 64-bit seed.
     */
    auto seed(const uint32_t dwSeed0, const uint32_t dwSeed1) -> void
    {
        uint64_t dwSeed01 = uint64_t(dwSeed0) | (uint64_t(dwSeed1) << 32);
        return seed(dwSeed01);
    }

    /**
     * @brief Returns a random number.
     */
    auto operator()() -> result_type
    {
        const result_type result = Rotl(m_nqwState[0] + m_nqwState[3], 23) + m_nqwState[0];

        const result_type t = m_nqwState[1] << 17;

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

/**
 * @brief Set the seed of the passed RNG based on std::random_device.
 *
 * @param cRNG RNG to seed.
 */
auto SetEntropySeed(Xoshiro256pp &cRNG) -> void;

/**
 * @brief Set the seed of the global RNG based on std::random_device.
 */
auto SetEntropySeed() -> void;

/**
 * @brief Set the seed of the passed RNG based on an unsigned 64-bit
 *        seed.
 *
 * @param cRNG RNG to seed.
 * @param qwSeed Seed to initialize state with.  Uses SplitMix64 to stretch
 *        the seed across all 256 bits of state.
 */
auto SetSeed(Xoshiro256pp &cRNG, const uint64_t qwSeed) -> void;

/**
 * @brief Set the seed of the global RNG based on an unsigned 64-bit
 *        seed.
 *
 * @param qwSeed Seed to initialize state with.  Uses SplitMix64 to stretch
 *        the seed across all 256 bits of state.
 */
auto SetSeed(const uint64_t qwSeed) -> void;

/**
 * @brief Return a 32-bit integer from the passed RNG.
 *
 * @param cRNG RNG to use.
 * @return A random 32-bit integer.
 */
auto U32(Xoshiro256pp &cRNG) -> uint32_t;

/**
 * @brief Return a 32-bit integer from the global RNG.
 *
 * @return A random 32-bit integer.
 */
auto U32() -> uint32_t;

/**
 * @brief Return a random uniformly-distributed integer from the passed RNG.
 *
 * @param cRNG RNG to use.
 * @param qwUpperBound One more than the maximum number we want to return.
 * @return A random integer in the half-open range [0, Upper Bound).
 */
auto UniformU32(Xoshiro256pp &cRNG, const uint32_t qwUpperBound) -> uint32_t;

/**
 * @brief Return a random uniformly-distributed integer from the passed RNG.
 *
 * @param qwUpperBound One more than the maximum number we want to return.
 * @return A random integer in the half-open range [0, Upper Bound).
 */
auto UniformU32(const uint32_t qwUpperBound) -> uint32_t;

/**
 * @brief Return a 64-bit integer from the passed RNG.
 *
 * @param cRNG RNG to use.
 * @return A random 64-bit integer.
 */
auto U64(Xoshiro256pp &cRNG) -> uint64_t;

/**
 * @brief Return a 64-bit integer from the global RNG.
 *
 * @return A random 64-bit integer.
 */
auto U64() -> uint64_t;

/**
 * @brief Return a random uniformly-distributed integer from the passed RNG.
 *
 * @param cRNG RNG to use.
 * @param qwUpperBound One more than the maximum number we want to return.
 * @return A random integer in the half-open range [0, Upper Bound).
 */
auto UniformU64(Xoshiro256pp &cRNG, const uint64_t qwUpperBound) -> uint64_t;

/**
 * @brief Return a random uniformly-distributed integer from the passed RNG.
 *
 * @param qwUpperBound One more than the maximum number we want to return.
 * @return A random integer in the half-open range [0, Upper Bound).
 */
auto UniformU64(const uint64_t qwUpperBound) -> uint64_t;

/**
 * @brief Return a random floating point number from the passed RNG.
 *
 * @param cRNG RNG to use.
 * @return A random floating point number in the half-open range [0.0, 1.0).
 */
auto Float(Xoshiro256pp &cRNG) -> float;

/**
 * @brief Return a random floating point number from the global RNG.
 *
 * @return A random floating point number in the half-open range [0.0, 1.0).
 */
auto Float() -> float;

} // namespace rock3d::Random
