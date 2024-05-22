#pragma once

#include <gmpxx.h>

#include "global.hpp"

namespace Lexi {
enum Res { OK, NOT_OK };
mint rank(std::string bit_string, int k, int n);
[[nodiscard]] std::pair<std::string, Lexi::Res> unrank(mint pos, int k, int n);
std::array<int, 2> get_k_n(std::string bit_string);
} // namespace Lexi
