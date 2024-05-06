#include "xoshiro256.hpp"
#include <random>

// seed generator once
inline std::random_device rd;
inline xoshiro256ss g_256ss(rd());
