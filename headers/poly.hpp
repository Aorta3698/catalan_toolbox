#pragma once

#include "global.hpp"
#include "tree.hpp"
#include <string>

class Tree;

class Poly {
public:
  Poly(Graph poly) {
    this->poly = poly; // poly includes the root top side
    this->sides = int(poly.size()) + 2;
  }

  /**
   * Create a random polygon triangulation
   *
   * @param num_of_sides:   Number of sides
   *
   * @return A random polygon triangulation
   */
  static Poly *get_random(int num_of_sides);

  /**
   * Verify if the given graph is polygon triangulation.
   *
   * @return True if it is a valid polygon triangulation, false otherwise
   */
  static bool is_valid(Graph &);

  /**
   * Transform to its tree representation.
   *
   * @return The root of the tree represented by the polygon
   */
  Tree *to_tree();

  /**
   * Plot the current polygon triangulation
   */
  void plot(std::string file = "");

  /**
   * Get the next polygon triangulation
   * @return The next polygon triangulation
   */
  Poly *next();

  /**
   * Flip an edge of a polygon triangulation until the user quits
   * It takes O(n) pre-processing time and O(1) per edge flip.
   */
  void flip_and_plot();

  /**
   * Test poly <-> tree conversion with 3 <= num_of_sides <= TEST_MAX_SIDES_POLY
   * (default 100). There are NUM_OF_TESTS_POLY (default 50k) random test cases for
   * each `num_of_sides`.
   */
  void test_conversion();

private:
  static constexpr std::string _PLOT_SCRIPT = "plot-poly.py";
  static constexpr std::string _DEFAULT_PREFIX_FILE = ".poly";
  static constexpr std::string _WATCHDOG_FILE = ".polylog";
  const static int NUM_OF_TESTS{50000};
  const static int TEST_MAX_SIDES{100};

  Graph poly;
  int sides;
};
