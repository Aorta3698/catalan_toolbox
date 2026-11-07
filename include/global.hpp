#pragma once

#include "xoshiro256.hpp"

// #include <gmpxx.h>
#include <random>

// seed generator once
inline std::random_device rd;
inline xoshiro256pp g_256pp(rd());

using Edge = std::array<int, 2>;
using Graph = std::vector<Edge>;
// using mint = mpz_class;
