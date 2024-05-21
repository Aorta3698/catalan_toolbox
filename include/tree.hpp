#pragma once

#include <memory>
#include <string>
#include <vector>

#include "arcs.hpp"
#include "catalan.hpp"
#include "chords.hpp"
#include "coin.hpp"
#include "dyck_mirrored.hpp"
#include "dyck_pre.hpp"
#include "mutze_tree.hpp"
#include "poly.hpp"

// forward declaration
// https://stackoverflow.com/a/29209540
class Poly;
class Chords;
class DyckPre;
class DyckPreMirrored;
class Arcs;
class CoinStack;

/**
 * Node class represents a tree node
 *
 */
struct Node {
  Node(int id, int node_count = 0) {
    this->id = id;
    children = std::vector<std::unique_ptr<Node>>(node_count);
  }

  inline bool is_internal_node() const { return !children.empty(); }
  inline bool is_leaf() const { return !is_internal_node(); }
  inline int child_count() const { return int(children.size()); }

  // use raw pointer for parent if needed:
  // https://www.reddit.com/r/cpp_questions/comments/njqjlk/building_a_tree_with_smart_pointers_parental/
  // https://codereview.stackexchange.com/a/191297
  std::vector<std::unique_ptr<Node>> children;
  int id;
};

class Tree : public Catalan<Tree> {
public:
  enum TreeType { Non_Full, Full };

  Tree(std::unique_ptr<Node> root, TreeType type = TreeType::Full) {
    this->k = root->child_count();
    this->root = std::move(root);
    this->type = type;
  }

  // loved c++20
  auto operator<=>(const Tree &rhs) const = default;

  /**
   * From degree 2 to 5 (due to time), sample 10k random trees with
   * 100k internal nodes, and then compare it to the expected height.
   *
   * Assert in error if they differ by more than 1%.
   */
  static void test_expected_height();

  /**
   * Create a random full k-ary tree, which corresponds
   * to a valid dyck path.
   *
   * @param deg:                The degree of the tree
   * @param num_of_full_nodes:  Number of internal nodes
   *
   * @return The root of the generated tree
   */
  static std::unique_ptr<Tree> get_random(int deg, int num_of_full_nodes);

  /**
   * Read the file that consists of one edge on each line
   * with both vertex separated by a comma, and then reconstruct
   * the full k-ary tree represented by it.
   *
   * Throw an exception if the file can not be opened or
   * it is not a full k-ary tree
   *
   * @param file:   The file containing the full k-ary tree
   *
   * @return The full k-ary tree represented by the file
   */
  static std::unique_ptr<Tree> get_from_file(std::string file);

  /**
   * The asymptotic, average height of a k-ray tree with
   * num_of_internal_nodes internal nodes.
   *
   * @param k:                     Degree of the tree
   * @param num_of_internal_nodes: Number of internal nodes
   *
   * @return The asymptotic, average height of the tree
   */
  static double asymptote(int k, int num_of_internal_nodes);

  /**
   * Build a binary tree from Mutze avoiding tree.
   *
   * @param mutze_tree: A mutze_tree with avoiding patterns loaded.
   */
  static std::unique_ptr<Tree> get_from_Mutze(const Mutze::Tree &mutze_tree);

  /**
   * Constructor a catalan structure from the current mutze tree.
   *
   * @param mtree:  Mutze tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<Tree> of(const Mutze::Tree &mtree);

  /**
   * Convert a tree into its coin stack representation
   *
   * @return Its coin stack object.
   */
  std::unique_ptr<CoinStack> to_coin_stack();

  /**
   * Convert a tree into its mirrored pre-order dyck path representation
   *
   * @return Its DyckPreMirrored object.
   */
  std::unique_ptr<DyckPreMirrored> to_dyck_pre_mirrored();

  /**
   * Convert the current tree into
   * its arcs graph representation
   *
   * @return Its arcs graph representation
   */
  std::unique_ptr<Arcs> to_arcs();

  /**
   * Convert the current tree into its
   * polygon triangulation representation
   *
   * @return Its polygon triangulation representation
   */
  std::unique_ptr<Poly> to_poly();

  /**
   * Convert the current tree into its chords graph representation
   *
   * @return Its chords graph representation
   */
  std::unique_ptr<Chords> to_chords();

  /**
   * Convert a tree into its pre-order dyck path representation
   *
   * @return Its DyckPre representation.
   */
  std::unique_ptr<DyckPre> to_dyck_pre();

  /**
   * Store tree into a file with one edge on each line,
   * and both vertex separated by a comma.
   *
   * It throws an exception if the file given
   * can not be opened.
   *
   * @param file:  Output filename
   */
  void to_file(std::string file = "");

  /**
   * Rotate into the next tree.
   * @return The next tree.
   */
  std::unique_ptr<Tree> next();

  /**
   * Serializes the current tree into string.
   * Different tree has its own unique serialization.
   *
   * @return The unique serialization of the tree
   */
  std::string serialize();

  /**
   * Compute the height of the tree - root by itself has a height of 1.
   *
   * @return Height of the tree
   */
  int height();

  /**
   * Enumerate *k-ary* trees lexicographically.
   */
  void enumerate_lexico();

  static constexpr std::string _DEFAULT_PREFIX_FILE{".tree"};
  static constexpr std::string _DEFAULT_DB_FILE{".tree_db"};
  static constexpr std::string _PLOT_SCRIPT{"plot-tree.py"};

private:
  std::unique_ptr<Node> root;
  TreeType type;
  int k;
};
