#pragma once

#include "coin.hpp"
#include "tree.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

class Tree;
class CoinStack;

class DyckPre {
public:
  DyckPre(std::string path) {
    this->path = path;
    this->length = int(path.size());
    this->r = this->length / std::ranges::count(path, '0');

    if (this->length % this->r) {
      std::cerr << std::format(
          "Error: DyckPre constructor received an invalid dyck path {}\n", path);
      throw std::invalid_argument("");
    }
  }

  // loved c++20
  auto operator<=>(const DyckPre &rhs) const = default;

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
   * Get a random Dyck Path in pre-order
   *
   * @param r:   Degree of the path
   * @param len:   Length of the path
   *
   * @return A DyckPre (dyck path in pre-order)
   */
  static std::unique_ptr<DyckPre> get_random(int r, int len);

  /**
   * Verify if the given string is a valid dyck path or not
   *
   * @param path:  A string in 01
   *
   * @return True if it is a valid dyck path, false otherwise
   */
  static bool is_valid(const std::string &path);

  /**
   * Transform a dyck path to its pre-order tree representation.
   * If the dyck path is not valid, throw an exception.
   *
   * @return The root of the tree represented by the dyck path
   */
  std::unique_ptr<Tree> to_tree();

  /**
   * Transform a dyck path to its coin stack representation.
   * If the dyck path is not valid, throw an exception.
   *
   * @return The coin stack represented by the dyck path
   */
  std::unique_ptr<CoinStack> to_coin_stack();

  /**
   * Get the next dyck path in pre-order
   *
   * @return The next dyck path in pre-order
   */
  std::unique_ptr<DyckPre> next();

  /**
   * Plot the current dyck path (above the line)
   *
   * @param file: Where the current DyckPre would be stored - _DEFAULT_PREFIX_FILE by
   * default
   */
  void plot(std::string file = "");

  /**
   * Flip a mountain of the current dyck path
   */
  void flip_mountain();

  inline std::string get_path() const { return this->path; }
  inline int get_r() const { return this->r; }
  inline int size() const { return this->length; }
  inline void print() const { std::cout << this->path << "\n"; }

private:
  static constexpr std::string_view _PLOT_SCRIPT{"plot-dyck-path.py"};
  static constexpr std::string _DEFAULT_PREFIX_FILE{".dyck"};
  static constexpr int _NUM_OF_TESTS = 1e5;
  static constexpr int _TEST_MAX_EDGES = 1e4;
  static constexpr int _TEST_MAX_BRANCHES = 1e2;

  std::string path;
  int length;
  int r;
};
