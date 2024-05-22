#pragma once

#include <memory>
#include <string>

#include "arcs.hpp"
#include "base_tree.hpp"
#include "catalan.hpp"
#include "chords.hpp"
#include "coin.hpp"
#include "dyck_mirrored.hpp"
#include "dyck_pre.hpp"
#include "poly.hpp"

// forward declaration
// https://stackoverflow.com/a/29209540
class Poly;
class Chords;
class DyckPre;
class DyckPreMirrored;
class Arcs;
class CoinStack;

class Tree : public Catalan<Tree>, public BaseTree {

public:
  Tree(std::unique_ptr<BaseTree> tree) : BaseTree(std::move(tree)){};

  Tree(std::unique_ptr<Node> root, TreeType type = TreeType::Full)
      : BaseTree(std::move(root), type) {}

  auto operator<=>(const Tree &rhs) const = default;

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
   * Build a binary tree from the traversal sequence produnced by Mutze avoiding
   * tree.
   *
   * @param traversal: An in-order, pre-order traversal sequence
   */
  static std::unique_ptr<Tree> get_from_traversal(const std::string &traversal);

  /**
   * Construct a catalan structure from the current mutze tree.
   *
   * @param mtree:  Mutze tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<Tree> of(std::unique_ptr<BaseTree> base_tree);

  /**
   * Construct a catalan structure from the 01 bitstring
   *
   * @param bitstring: 01 bitstring in pre-order traversal
   *
   * @return The k-ary tree represented by the bitstring
   */
  static std::unique_ptr<Tree> get_from_bitstring(const std::string &bitstring);

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
   * Rotate into the next tree.
   * @return The next tree.
   */
  std::unique_ptr<Tree> next();

  static constexpr std::string _DEFAULT_PREFIX_FILE{".tree"};
  static constexpr std::string _DEFAULT_DB_FILE{".tree_db"};
  static constexpr std::string _PLOT_SCRIPT{"plot-tree.py"};

private:
};
