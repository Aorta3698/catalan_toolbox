#pragma once

#include "xoshiro256.hpp"

#include <random>

// seed generator once
inline std::random_device rd;
inline xoshiro256ss g_256ss(rd());

using Edge = std::array<int, 2>;
using Graph = std::vector<Edge>;

// enum Cat { Tree, Poly, Chords, Arcs, Coins, Dyck };
