#pragma once

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

  std::vector<Node *> children;
  int id;
};

/**
 * Plot all the trees with n internal nodes.
 * It only accepts at most 4 internal nodes.
 *
 * @param n:  Number of internal nodes
 */
void plot_all_trees(int n);

/**
 * Create a random full k-ary tree, which corresponds
 * to a valid dyck path.
 *
 * @param deg:                The degree of the tree
 * @param num_of_full_nodes:  Number of internal nodes
 *
 * @return The root of the generated tree
 */
Node *get_random_tree(int deg, int num_of_full_nodes);

/**
 * Store a tree into a file with one edge on each line,
 * and both vertex separated by a comma.
 *
 * It throws an exception if the file given
 * can not be opened.
 *
 * @param root:  Root of the tree
 * @param file:  Output filename
 */
void store_tree_into_file(const Node *root, std::string file);

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
 * @return Root of the full k-ary tree represented by the file
 */
Node *get_tree_from_file(std::string file);

/**
 * Plot a tree with networkx from python.
 *
 * The file consists of one edge on each line
 * with both vertex separated by a comma.
 *
 * @param file:  The file that contains the tree
 */
void plot_tree(std::string file);

/**
 * It serializes a tree into string.
 * Different tree has its own unique serialization.
 *
 * @param root: Root of the tree
 * @return The unique serialization of the tree
 */
std::string serialize_tree(const Node *root);

/**
 * Free memory used by the tree nodes.
 *
 * @param root: Root of a tree
 */
void free_tree(Node *root);

/**
 * Compute the height of the tree - root by itself has a height of 1.
 *
 * @param root:  Root of the tree
 *
 * @return Height of the tree
 */
int height(const Node *);

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
