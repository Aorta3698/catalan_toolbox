#pragma once

#include "global.hpp"
#include "tree.hpp"
#include <string>

const static int NUM_OF_TESTS_POLY{50000};
const static int TEST_MAX_SIDES_POLY{100};

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
   * Plot all polygon triangulation with `num_of_sides` sides
   *
   * @param num_of_sides:  Number of sides for the polygon. Between 3 to 6,
   * inclusive.
   */
  // void plot_all_poly(int num_of_sides);

  /**
   * Get the next polygon triangulation
   * @return The next polygon triangulation
   */
  Poly *next();

  /**
   * Plot `count` number of random polygon triangulation
   *
   * @param num_of_sides:   Number of sides
   * @param count:          Number of random polygon plots
   */
  // void plot_random_poly(int num_of_sides, int count);

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

  Graph poly;
  int sides;
};
