#include "global.hpp"
#include "tree.hpp"

const static int NUM_OF_TESTS_ARCS{50000};
const static int TEST_MAX_SIDES_ARCS{200};

/**
 * Convert a tree into its arcs graph epresentation
 *
 * @param root:     The root of the tree
 *
 * @return Its arcs graph representation
 */
Arcs tree_to_arcs(const Node *root);

/**
 * Transform a arcs graph to its tree representation.
 *
 * @param arcs:  An arcs graph
 *
 * @return The root of the tree represented by the arcs graph
 */
Node *arcs_to_tree(const Arcs arcs);

/**
 * Plot a arcs graph
 *
 * @param arcs:  An arcs graph
 */
void plot_arcs(const Arcs arcs, std::string file);

/**
 * Plot all arcs graph with `num_of_points` points
 *
 * @param num_of_points:  Number of points for the arcs graph. Between 2 to 8,
 * inclusive.
 */
void plot_all_arcs(int num_of_points);

/**
 * Create a random arcs graph
 *
 * @param num_of_points:   Number of points
 *
 * @return A random arcs graph
 */
Arcs get_random_arcs(int num_of_points);

/**
 * Plot `count` number of random arcs graph
 *
 * @param num_of_sides:   Number of sides
 * @param count:          Number of random arcs graph plots
 */
void plot_random_arcs(int num_of_points, int count);

/**
 * Exchange 2 arcs until the user quits.
 * It takes O(n) pre-processing time and O(1) per exchage.
 *
 * @param: arcs   An arcs graph
 */
void exchage_arcs(Arcs arcs);

/**
 * Verify if an arcs graph is valid or not
 *
 * @param arcs:  An arcs graph
 *
 * @return True if it is a valid arcs graph, false otherwise
 */
bool is_valid_arcs(const Arcs arcs);

/**
 * Test arcs <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_arcs
 * (default 100). There are NUM_OF_TESTS_arcs (default 50k) random test cases for
 * each total number of points.
 */
void test_conversion_arcs();
