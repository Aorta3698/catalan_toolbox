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

#include <vector>

namespace Mutze {
class BaseTree {
public:
  int n_;                        // number of vertices in the binary tree
  int root_;                     // root of the binary tree
  std::vector<int> left_child_;  // left child of each vertex
  std::vector<int> right_child_; // right child of each vertex
  std::vector<int> parent_;      // parent of each vertex

  auto operator<=>(const BaseTree &rhs) const = default;

  void init();             // initializer
  void insert_leaf(int i); // insert vertex i as a new leaf into the search tree
  bool is_leaf(int i) const {
    return ((left_child_[i] == 0) and (right_child_[i] == 0));
  } // check if vertex i is a leaf
};
} // namespace Mutze
