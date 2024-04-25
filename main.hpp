#pragma once

#include <string>
#include <unistd.h>
#include <vector>

const static int ERROR = -1;
const static int NUM_OF_TESTS = 1e5;
const static int TEST_MAX_EDGES = 1e4;
const static int TEST_MAX_BRANCHES = 1e2;
const static char *TREE_OUTPUT_FILENAME = ".tree_db";

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

  std::vector<Node *> children;
  int id;
};

//
// ================= input processing ===================
//

/**
 * Print usage of the program to the console
 */
void print_usage();

/**
 * Split str by the given delimiter
 *
 * @param str:       Input string
 * @param delimiter: A delimiter of char type
 *
 * @return A vector containing the splitted string
 */
std::vector<std::string> split_string(std::string str, char delimiter);

/**
 * Convert a string to a positive integer. Throw an exception if
 * the input is not fully processed or if it is non-positive.
 *
 * @param str:   A number in string
 *
 * @return The converted number of int type
 */
int get_num(std::string str);

/**
 * Trim the whitespaces from both ends of a string.
 * It modifies it in place.
 *
 * @param str:   Input string
 *
 * @return A vector containing k random numbers
 */
std::string &strip(std::string &str);

//
// ================= main functions ===================
//

/**
 * Transform a dyck path to its tree representation.
 * If the dyck path is not valid, throw an exception.
 *
 * @param path:  A dyck path of string type
 *
 * @return The root of the tree represented by the dyck path
 */
Node *dyck_path_to_tree(std::string path);

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
 * Create a random Dyck Path
 *
 * @param deg:   Degree of the path
 * @param len:   Length of the path
 *
 * @return A dyck path of string type
 */
std::string get_random_dyck_path(int deg, int len);

/**
 * Convert a tree into its dyck path representation
 *
 * @param root:     The root of the tree
 *
 * @return Its dyck path of string type
 */
std::string tree_to_dyck_path(const Node *root);

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

//
// ================= clean up ===================
//

/**
 * Free memory used by the tree nodes.
 *
 * @param root: Root of a tree
 */
void free_tree(Node *root);

//
// ================= helper functions ===================
//

/**
 * Print a vector to console
 *
 * @param vec: The vector in question
 */
template <typename T> void print_vector(const T &vec);

/**
 * Print a C array to console
 *
 * @param arr: The C-style array in question
 * @param len: Length of the C-style array
 */
template <typename T> void print_c_array(const T *arr, int len);

/**
 * Verify if a dyck path is valid or not
 *
 * @param path:  Dyck path of string type
 *
 * @return True if it is a valid dyck path, false otherwise
 */
bool is_valid_dyck_path(std::string path);

//
// ================= test cases ===================
//

/**
 * 100,000 test cases of creating a random dyck path, converting it to tree
 * and back to dyck path, then comparing if they are identical. By default,
 * the maximum of degree generated is 100 with a maximum of 10,000 edges.
 *
 * Every 4%, it prints the progress status to the console.
 *
 * In the end, it displays the stats of all test cases sorted by degrees
 * if all 100,000 test cases pass; otherwise, it asserts in error and displays
 * the discrepancy of the 2 dyck paths.
 */
void test_functionality();

/**
 * From degree 2 to 5 (due to time), sample 10k random trees with
 * 100k internal nodes, and then compare it to the expected height.
 *
 * Assert in error if they differ by more than 1%.
 */
void test_expected_height();

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
