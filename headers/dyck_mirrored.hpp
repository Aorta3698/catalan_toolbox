#pragma once

#include "tree.hpp"

#include <algorithm>
#include <string>

class Tree;

class DyckPreMirrored {
public:
  DyckPreMirrored(std::string path) {
    this->path = path;
    this->length = int(path.size());
    this->r = this->length / std::ranges::count(path, '0');

    if (this->length % this->r) {
      std::cerr << std::format(
          "Error: DyckPreMirrored constructor received an invalid dyck path {}\n",
          path);
      throw std::invalid_argument("");
    }
  }

  // loved c++20
  auto operator<=>(const DyckPreMirrored &rhs) const = default;

  /**
   * 100,000 test cases of creating a random dyck path, converting it to tree
   * and back to dyck path, then comparing if they are identical. By default,
   * the maximum of degree generated is 100 with a maximum of 10,000 edges.
   *
   * Every 4%, it prints the progress status to the console.
   *
   * In the end, it displays the stats of all test cases sorted by degrees
   * if all 100,000 test cases pass; otherwise, it asserts in error and displays
   * the discrepancy of the 2 dyck paths.
   */
  static void test_conversion();

  /**
   * Verify if a 01 bitstring is a valid dyck path in mirrored pre-order or not
   *
   * @param path:  A path in 01 bitstring
   *
   * @return True if it is a valid dyck path in mirrored pre-order, false otherwise
   */
  static bool is_valid(const std::string &path);

  /**
   * Get a random dyck path in mirrored pre-order.
   *
   * @param deg:   Degree of the path
   * @param len:   Length of the path
   *
   * @return A DyckPreMirrored object
   */
  static DyckPreMirrored *get_random(int deg, int len);

  /**
   * Transform the current dyck path to its tree representation.
   *
   * @return The root of the tree represented by the dyck path
   */
  Tree *to_tree();

  /**
   * Transform the current dyck path to its tree representation.
   *
   * And then delete itself
   *
   * @return The root of the tree represented by the dyck path
   */
  Tree *into_tree();

  /**
   * Get the next DyckPreMirrored object.
   *
   * @return The next DyckPreMirrored object.
   */
  DyckPreMirrored *next();

private:
  const static int _NUM_OF_TESTS = 1e5;
  const static int _TEST_MAX_EDGES = 1e4;
  const static int _TEST_MAX_BRANCHES = 1e2;

  std::string path;
  int length;
  int r;
};
