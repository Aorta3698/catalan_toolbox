#pragma once

#include "chords.hpp"
#include "global.hpp"
#include "tree.hpp"

class Tree;
class Chords;

class Arcs {
public:
  /**
   * Verify if the given graph is a valid Arcs graph or not
   *
   * @param graph:  A graph
   *
   * @return True if it is a valid arcs graph, false otherwise
   */
  static bool is_valid(const Graph &graph);

  /**
   * Create a random arcs graph
   *
   * @param num_of_points:   Number of points
   *
   * @return A random arcs graph
   */
  static Arcs *get_random(int num_of_points);

  Arcs(Graph arcs) {
    this->arcs = arcs;
    this->points = int(arcs.size()) << 1;
  }

  // loved c++20
  auto operator<=>(const Arcs &rhs) const = default;

  /**
   * Get the next Arcs graph
   *
   * @return The next Arcs graph
   */
  Arcs *next();

  /**
   * Transform the current arcs graph
   * to its tree representation.
   *
   * @return The tree represented by the arcs graph
   */
  Tree *to_tree();

  /**
   * Transform the current arcs graph
   * to its chords graph representation
   *
   * @return The chords graph represented by the arcs graph
   */
  Chords *to_chords();

  /**
   * Transform the current arcs graph
   * to its tree representation.
   *
   * And then delete itself
   *
   * @return The tree represented by the arcs graph
   */
  Tree *into_tree();

  /**
   * Transform the current arcs graph
   * to its chords graph representation
   *
   * And then delete itself
   *
   * @return The chords graph represented by the arcs graph
   */
  Chords *into_chords();

  /**
   * Plot the current arcs graph
   *
   * @param file:  The file to store the arcs graph. Default to _DEFAULT_PREFIX_FILE
   */
  void plot(std::string file = "");

  /**
   * Exchange 2 arcs until the user quits.
   * It takes O(n) pre-processing time and O(1) per exchage.
   */
  void exchage_arcs();

  /**
   * Test arcs <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_arcs
   * (default 100). There are NUM_OF_TESTS_arcs (default 50k) random test cases for
   * each total number of points.
   */
  void test_conversion();

private:
  static constexpr std::string _DEFAULT_PREFIX_FILE{".arcs"};
  static constexpr std::string _PLOT_SCRIPT{"plot-arcs.py"};
  const static int _NUM_OF_TESTS{50000};
  const static int _TEST_MAX_SIDES{200};

  Graph arcs;
  int points;
};
