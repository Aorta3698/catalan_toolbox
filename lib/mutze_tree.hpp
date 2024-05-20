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

#include "mutze_pattern.hpp"
#include <vector>

namespace Mutze {
// host tree class that avoids the forbidden pattern trees
class Tree : private BaseTree {
  enum class Direction {
    up,
    down
  }; // direction of rotation of a vertex in the binary tree

public:
  auto operator<=>(const Tree &rhs) const = default;
  Tree(int n, const std::vector<Pattern>
                  patterns); // create an all-right tree with n vertices that avoids
                             // given forbidden patterns
  bool next();               // compute the next tree that avoids a friendly pattern
  bool avoids();             // check if the tree avoids all the given patterns
  void print() const;        // print the preorder traversal of the tree
  static std::vector<Tree> construct_all_trees(
      int n,
      const std::vector<Pattern> patterns); // construct all binary trees with n
                                            // vertices that avoids given patterns

private:
  // bookkeeping for Algorithm S
  std::vector<Direction> o_;      // direction array
  std::vector<int> s_;            // vertex that slides
  std::vector<Pattern> patterns_; // list of forbidden patterns to be avoided

  void init_bookkeeping();         // initialize s_ and o_ arrays
  void init_tree();                // initialize the tree itself
  void aux_update(int j);          // update s_ and  o_ arrays
  void rotate(int j, Direction d); // rotate the vertex j in direction d
  void slide(int j); // slide the vertex j such that the new tree avoids all
                     // forbidden patterns
  bool contains(int node, int pnode, const Pattern &p,
                std::vector<std::vector<int>>
                    &ctab); // Check if the binary tree contains the given pattern.
                            // This is done by dynamic programming bottom-up in the
                            // tree, using the table ctab.
  void print_preorder(int data) const; // print the preorder sequence of the tree
  static Tree merge_trees(
      const Tree &left, const Tree &right, int i,
      const std::vector<Pattern> patterns); // the two trees are merged and become
                                            // left and right subtrees of i
  void increase_key(int d); // increase the value of every vertex by d
};
} // namespace Mutze
