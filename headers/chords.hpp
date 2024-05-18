#pragma once

#include "global.hpp"
#include "tree.hpp"

class Tree;

class Chords {
public:
  /**
   * Get a random chords graph
   *
   * @param num_of_points:   Number of points
   *
   * @return A random chords graph
   */
  static Chords *get_random(int num_of_points);

  /**
   * Verify if a given graph is a valid chords graph or not
   *
   * @param chords:  A chords graph
   *
   * @return True if it is a valid chords graph, false otherwise
   */
  static bool is_valid(const Graph &graph);

  Chords(Graph chords) {
    this->chords = chords;
    this->points = int(chords.size()) << 1;
  }

  /**
   * Transform the current chords to its tree representation.
   *
   * @param chords:  A chords graph
   *
   * @return The root of the tree represented by the chords graph
   */
  Tree *to_tree();

  /**
   * Get the next chords graph
   *
   * @return The root of the tree represented by the chords graph
   */
  Chords *next();

  /**
   * Plot the current chords graph
   */
  void plot(std::string file = "");

  /**
   * Exchange 2 chords until the user quits.
   * It takes O(n) pre-processing time and O(1) per exchage.
   */
  void exchage_chords();

  /**
   * Test chords <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_chords
   * (default 100). There are NUM_OF_TESTS_chords (default 50k) random test cases for
   * each total number of points.
   */
  void test_conversion();

private:
  static constexpr std::string _PLOT_SCRIPT{"plot-chords.py"};
  static constexpr std::string _DEFAULT_PREFIX_FILE{".chords"};
  const static int _NUM_OF_TESTS{50000};
  const static int _TEST_MAX_SIDES{200};

  Graph chords;
  int points;
};
