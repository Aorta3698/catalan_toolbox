#pragma once

#include <memory>

#include "arcs.hpp"
#include "catalan.hpp"
#include "global.hpp"
#include "tree.hpp"

class Tree;
class Arcs;

class Chords : public Catalan<Chords>, public OsOp {
public:
  Chords(Graph chords) : OsOp(_PLOT_SCRIPT, _DEFAULT_PREFIX_FILE) {
    this->chords = chords;
    this->points = int(chords.size()) << 1;
  }

  auto operator<=>(const Chords &rhs) const = default;

  /**
   * Test chords <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_chords
   * (default 100). There are NUM_OF_TESTS_chords (default 50k) random test cases for
   * each total number of points.
   */
  static void test_conversion();

  /**
   * Get a random chords graph
   *
   * @param num_of_points:   Number of points
   *
   * @return A random chords graph
   */
  static std::unique_ptr<Chords> get_random(int num_of_points);

  /**
   * Verify if a given graph is a valid chords graph or not
   *
   * @param chords:  A chords graph
   *
   * @return True if it is a valid chords graph, false otherwise
   */
  static bool is_valid(const Graph &graph);

  /**
   * Constructor a catalan structure from the current base tree
   *
   * @param tree:  current base tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<Chords> of(std::unique_ptr<BaseTree> tree);

  /**
   * Transform the current chords to its tree representation.
   *
   * @return The tree represented by the chords graph
   */
  std::unique_ptr<Tree> to_tree();

  /**
   * Transform the current chords to its arcs graph representation.
   *
   * @return The arcs graph represented by the chords graph
   */
  std::unique_ptr<Arcs> to_arcs();

  /**
   * Get the next chords graph
   *
   * @return The root of the tree represented by the chords graph
   */
  std::unique_ptr<Chords> next();

  /**
   * Store the current chords graph in a file
   * file by default is _DEFAULT_PREFIX_FILE
   *
   * @param file: The file to store it
   */
  void to_file(std::string file = "");

  /**
   * Exchange 2 chords until the user quits.
   * It takes O(n) pre-processing time and O(1) per exchage.
   */
  void exchage_chords();

  static constexpr std::string _PLOT_SCRIPT{"plot-chords.py"};
  static constexpr std::string _DEFAULT_PREFIX_FILE{".chords"};
  static constexpr int _NUM_OF_TESTS{20'000};
  static constexpr int _TEST_MAX_SIDES{200};

private:
  Graph chords;
  int points;
};
