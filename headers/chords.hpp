#include "global.hpp"
#include "tree.hpp"

const static int NUM_OF_TESTS_CHORDS{50000};
const static int TEST_MAX_SIDES_CHORDS{200};

/**
 * Convert a tree into its chords representation
 *
 * @param root:     The root of the tree
 *
 * @return Its chords graph representation
 */
Chords tree_to_chords(const Node *root);

/**
 * Transform a chords to its tree representation.
 *
 * @param chords:  A chords graph
 *
 * @return The root of the tree represented by the chords graph
 */
Node *chords_to_tree(const Chords chords);

/**
 * Plot a chords graph
 *
 * @param chords:  A chords graph
 */
void plot_chords(const Chords chords, std::string file);

/**
 * Plot all chords graph with `num_of_points` points
 *
 * @param num_of_points:  Number of points for the chords graph. Between 2 to 8,
 * inclusive.
 */
void plot_all_chords(int num_of_points);

/**
 * Create a random chords graph
 *
 * @param num_of_points:   Number of points
 *
 * @return A random chords graph
 */
Chords get_random_chords(int num_of_points);

/**
 * Plot `count` number of random chords graph
 *
 * @param num_of_sides:   Number of sides
 * @param count:          Number of random chords graph plots
 */
void plot_random_chords(int num_of_points, int count);

/**
 * Exchange 2 chords until the user quits.
 * It takes O(n) pre-processing time and O(1) per exchage.
 *
 * @param: chords   A chords graph
 */
void exchage_chords(Chords chords);

/**
 * Verify if a chords graph is valid or not
 *
 * @param chords:  A chords graph
 *
 * @return True if it is a valid chords graph, false otherwise
 */
bool is_valid_chords(const Chords chords);

/**
 * Test chords <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_chords
 * (default 100). There are NUM_OF_TESTS_chords (default 50k) random test cases for
 * each total number of points.
 */
void test_conversion_chords();
