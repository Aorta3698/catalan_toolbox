#include "tree.hpp"

#include <string>

const static int NUM_OF_TESTS = 1e5;
const static int TEST_MAX_EDGES = 1e4;
const static int TEST_MAX_BRANCHES = 1e2;

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
 * Verify if a dyck path is valid or not
 *
 * @param path:  Dyck path of string type
 *
 * @return True if it is a valid dyck path, false otherwise
 */
bool is_valid_dyck_path(std::string path);

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
