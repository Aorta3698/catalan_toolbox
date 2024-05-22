#pragma once

#include <memory>
#include <string>
#include <vector>

#include "os_op.hpp"

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

class BaseTree : public OsOp {

public:
  enum TreeType { Non_Full, Full };
  BaseTree(std::unique_ptr<BaseTree> o) : BaseTree(std::move(o->root), o->type){};
  BaseTree(std::unique_ptr<Node> root, TreeType type = TreeType::Full)
      : OsOp(_PLOT_SCRIPT, _DEFAULT_PREFIX_FILE) {
    this->k = root->child_count();
    this->root = std::move(root);
    this->type = type;
  }

  auto operator<=>(const BaseTree &rhs) const = default;

  /**
   * From degree 2 to 5 (due to time), sample 10k random trees with
   * 100k internal nodes, and then compare it to the expected height.
   *
   * Assert in error if they differ by more than 1%.
   */
  static void test_expected_height();

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
   * Get the bitstring representation of the current k-ary tree
   *
   * @return 01 bitstring in pre-order traversal
   */
  std::string to_bitstring();

  /**
   * Store tree into a file with one edge on each line,
   * and both vertex separated by a comma.
   *
   * It throws an exception if the file given
   * can not be opened.
   *
   * @param file:  Output filename
   */
  void to_file(FileOp mode = FileOp::Truncate, std::string file = "");

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
   * Build a binary tree from the traversal sequence produnced by Mutze avoiding
   * tree.
   *
   * @param traversal: An in-order, pre-order traversal sequence
   */
  static std::unique_ptr<BaseTree> get_from_traversal(const std::string &traversal);

  static constexpr std::string _DEFAULT_PREFIX_FILE{".basetree"};
  static constexpr std::string _DEFAULT_DB_FILE{".basetree_db"};
  static constexpr std::string _PLOT_SCRIPT{"plot-tree.py"};

protected:
  /**
   * Create a random full k-ary tree, which corresponds
   * to a valid dyck path.
   *
   * @param deg:                The degree of the tree
   * @param num_of_full_nodes:  Number of internal nodes
   *
   * @return The root of the generated tree
   */
  static std::unique_ptr<BaseTree> get_random(int deg, int num_of_full_nodes);

  /**
   * Construct a catalan structure from the 01 bitstring
   *
   * @param bitstring: 01 bitstring in pre-order traversal
   *
   * @return The k-ary tree represented by the bitstring
   */
  static std::unique_ptr<BaseTree> get_from_bitstring(const std::string &bitstring);

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
  static std::unique_ptr<BaseTree> get_from_file(std::string file);

  std::unique_ptr<Node> root;
  TreeType type;
  int k;
};
