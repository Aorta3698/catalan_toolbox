#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Node class represents a tree node
 *
 */
struct Node {
  // use pointer for now
  Node(int id, int node_count = 0) {
    this->id = id;
    children = std::vector<Node *>(node_count);
  }

  inline bool is_internal_node() const { return !children.empty(); }
  inline bool is_leaf() const { return !is_internal_node(); }
  inline int child_count() const { return int(children.size()); }

  std::vector<Node *> children;
  int id;
};

class Tree {
public:
  Tree(Node *root) {
    this->k = root->child_count();
    this->root = root;
  }

  Tree(int k, std::string pattern = "") {
    if (pattern.size() && k != 2) {
      std::cerr << "avoid patterns can only be given when it is a binary tree\n";
      throw std::invalid_argument("");
    }
    this->avoid_pattern = pattern;
  }

  /**
   * Create a random full k-ary tree, which corresponds
   * to a valid dyck path.
   *
   * @param deg:                The degree of the tree
   * @param num_of_full_nodes:  Number of internal nodes
   *
   * @return The root of the generated tree
   */
  Tree static *get_random(int deg, int num_of_full_nodes);

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
  Tree static *get_from_file(std::string file);

  /**
   * Plot all the trees with n internal nodes.
   * It only accepts at most 4 internal nodes.
   *
   * @param n:  Number of internal nodes
   */
  // void plot_all_trees(int n);

  /**
   * Store tree into a file with one edge on each line,
   * and both vertex separated by a comma.
   *
   * It throws an exception if the file given
   * can not be opened.
   *
   * @param file:  Output filename
   */
  void store_into_file(std::string file);

  /**
   * Plot the current tree.
   * @param file:   The output file. By default, it is _DEFAULT_FILE_PREFIX
   */
  void plot(std::string file = "");

  /**
   * Rotate into the next tree.
   * @return The next tree.
   */
  Tree *next();

  /**
   * Serializes the current tree into string.
   * Different tree has its own unique serialization.
   *
   * @return The unique serialization of the tree
   */
  std::string serialize();

  /**
   * Free memory used by the tree.
   */
  void free_memory();

  /**
   * Compute the height of the tree - root by itself has a height of 1.
   *
   * @return Height of the tree
   */
  int height();

  /**
   * The asymptotic, average height of a k-ray tree with
   * num_of_internal_nodes internal nodes.
   *
   * @param k:                     Degree of the tree
   * @param num_of_internal_nodes: Number of internal nodes
   *
   * @return The asymptotic, average height of the tree
   */
  double asymptote(int k, int num_of_internal_nodes);

private:
  static constexpr std::string _DEFAULT_FILE_PREFIX{".tree"};
  static constexpr std::string _PLOT_SCRIPT{"plot-tree.py"};

  std::string avoid_pattern;
  int k;
  Node *root;
};
