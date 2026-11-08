#pragma once

#include "poly.hpp"
#include "tree.hpp"

#include <memory>
#include <string>

class Poly;
class Tree;

class Bracketing {
public:
  Bracketing(std::string bracketing) { this->bracketing = bracketing; }

  auto operator<=>(const Bracketing &rhs) const = default;

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

  static constexpr int _NUM_OF_TESTS{20'000};
  static constexpr int _TEST_MAX_SIDES{100};

private:
  std::string bracketing;
};
