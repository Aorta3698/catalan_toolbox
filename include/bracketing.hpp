#pragma once

#include "os_op.hpp"
#include "poly.hpp"
#include "tree.hpp"

#include <memory>
#include <string>

class Poly;
class Tree;

class Bracketing : public OsOp {
public:
  Bracketing(std::string bracketing)
      : OsOp(std::string{_PLOT_SCRIPT}, _DEFAULT_PREFIX_FILE) {
    this->bracketing = bracketing;
  }

  auto operator<=>(const Bracketing &rhs) const = default;

  /**
   * Create a random bracketing
   *
   * @param num_of_factors:   Number of factors
   *
   * @return A random bracketing
   */
  static std::unique_ptr<Bracketing> get_random(int num_of_factors);

  /**
   * Verify if the given string is a valid bracketing or not
   *
   * @param bracketing:  A string
   *
   * @return True if it is a valid bracketing, false otherwise
   */
  static bool is_valid(const std::string &bracketing);

  /**
   * Test bracketing <-> tree conversion with 3 <= num_of_sides <= _TEST_MAX_SIDES
   * (default 100). There are _NUM_OF_TESTS (default 20k) random test cases for
   * each `num_of_sides`.
   */
  static void test_conversion();

  /**
   * Transform to its tree representation.
   *
   * @return The tree represented by the bracketing
   */
  std::unique_ptr<Tree> to_tree();

  /**
   * Transform to its polygon triangulation representation.
   *
   * @return The polygon triangulation represented by the bracketing
   */
  std::unique_ptr<Poly> to_poly();

  /**
   * Store the current bracketing into a file
   * file by default is _DEFAULT_PREFIX_FILE
   *
   * @param file: The file to store it
   */
  void to_file(FileOp mode = FileOp::Truncate, std::string file = "");

  static constexpr std::string _DEFAULT_PREFIX_FILE{".bracketing"};
  static constexpr std::string_view _PLOT_SCRIPT{"plot-bracketing.py"};
  static constexpr int _NUM_OF_TESTS{20'000};
  static constexpr int _TEST_MAX_SIDES{100};

private:
  std::string bracketing;
};
