/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

#pragma once

#include <cstdint>

#include <array>
#include <cmath>
#include <random>
#include <string_view>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "bgfx/bgfx.h"
#include "fmt/format.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "./vendor/nonstd/expected.hpp"
#include "./vendor/nonstd/scope.hpp"
#include "./vendor/nonstd/span.hpp"

#define ROCK3D_NOCOPY(cls)                                                                                             \
    cls(const cls &) = delete;                                                                                         \
    const cls &operator=(const cls &) = delete;                                                                        \
    static_assert(true, "")

#define ROCK3D_NOMOVE(cls)                                                                                             \
    cls(cls &&) = delete;                                                                                              \
    void operator=(cls &&) = delete;                                                                                   \
    static_assert(true, "")

namespace rock3d
{
using buffer_t = std::vector<uint8_t>;
}

#include "./util.h"
#include "./event.h"
#include "./level.h"
#include "./mathlib.h"
#include "./random.h"

#include "./platform.h"

#include "./assets.h"
#include "./engine.h"
#include "./renderUtils.h"
#include "./r3d/textures.h"
#include "./r3d/render.h"
