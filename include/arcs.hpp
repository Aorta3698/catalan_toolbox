#pragma once

#include "catalan.hpp"
#include "chords.hpp"
#include "global.hpp"
#include "tree.hpp"
#include <memory>

class Tree;
class Chords;

class Arcs : public Catalan<Arcs> {
public:
  Arcs(Graph arcs) {
    this->arcs = arcs;
    this->points = int(arcs.size()) << 1;
  }

  auto operator<=>(const Arcs &rhs) const = default;

  /**
   * Constructor a catalan structure from the current mutze tree.
   *
   * @param mtree:  Mutze tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<Arcs> of(const Mutze::Tree mtree);

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
  static std::unique_ptr<Arcs> get_random(int num_of_points);

  /**
   * Test arcs <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_arcs
   * (default 100). There are NUM_OF_TESTS_arcs (default 50k) random test cases for
   * each total number of points.
   */
  static void test_conversion();

  /**
   * Get the next Arcs graph
   *
   * @return The next Arcs graph
   */
  std::unique_ptr<Arcs> next();

  /**
   * Transform the current arcs graph
   * to its tree representation.
   *
   * @return The tree represented by the arcs graph
   */
  std::unique_ptr<Tree> to_tree();

  /**
   * Transform the current arcs graph
   * to its chords graph representation
   *
   * @return The chords graph represented by the arcs graph
   */
  std::unique_ptr<Chords> to_chords();

  /**
   * Store the current arcs graph as a file
   *
   * @param file:  The file to store it. Default to _DEFAULT_PREFIX_FILE
   */
  void to_file(std::string file = "");

  /**
   * Exchange 2 arcs until the user quits.
   * It takes O(n) pre-processing time and O(1) per exchage.
   */
  void exchage_arcs();

  static constexpr std::string _DEFAULT_PREFIX_FILE{".arcs"};
  static constexpr std::string _PLOT_SCRIPT{"plot-arcs.py"};
  static constexpr int _NUM_OF_TESTS{20'000};
  static constexpr int _TEST_MAX_SIDES{200};

private:
  Graph arcs;
  int points;
};
