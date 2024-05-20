#pragma once

/*
 * Copyright (c) 2023 Torsten Muetze and Namrata
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mutze_basetree.hpp"
#include <string>
#include <vector>

namespace Mutze {
// forbidden pattern tree class
class Pattern : public BaseTree {
public:
  enum class Edgetype { cont, semi_cont, non_cont }; // edge type
  std::vector<int> pat_;                             // list of vertices in preorder
  std::vector<int> e_pre_;  // edge type array indexed by preorder list of vertices
  std::vector<Edgetype> e_; // edge type array indexed by vertex names

  auto operator<=>(const Pattern &rhs) const = default;

  Pattern(const std::vector<int> &pat_pre, const std::vector<int> &e_pre);
  static bool read_patterns(std::string s, std::vector<Pattern> &patterns);
  bool is_friendly(); // check if the pattern is friendly
  bool avoids_231();  // check if the pattern is 231-avoiding

private:
  void init_pattern(); // initializer
};
} // namespace Mutze
