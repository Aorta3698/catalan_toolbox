#pragma once

#include "global.hpp"
#include "tree.hpp"
#include <string>

const static int NUM_OF_TESTS_POLY{50000};
const static int TEST_MAX_SIDES_POLY{100};

/**
 * Convert a tree into its poly representation
 *
 * @param root:     The root of the tree
 *
 * @return Its polygon triangulation represtation
 */
Poly tree_to_poly(const Node *root);

/**
 * Transform a poly to its tree representation.
 *
 * @param poly:  A polygon triangulation
 *
 * @return The root of the tree represented by the polygon
 */
Node *poly_to_tree(const Poly poly);

/**
 * Plot a polygon triangulation
 *
 * @param poly:  A polygon triangulation
 */
void plot_poly(Poly poly, std::string file);

/**
 * Plot all polygon triangulation with `num_of_sides` sides
 *
 * @param poly:  Number of sides for the polygon. Between 3 to 6, inclusive.
 */
void plot_all_poly(int num_of_sides);

/**
 * Create a random polygon triangulation
 *
 * @param num_of_sides:   Number of sides
 *
 * @return A random polygon triangulation
 */
Poly get_random_poly(int num_of_sides);

/**
 * Plot `count` number of random polygon triangulation
 *
 * @param num_of_sides:   Number of sides
 * @param count:          Number of random polygon plots
 */
void plot_random_poly(int num_of_sides, int count);

/**
 * Flip an edge of a polygon triangulation until the user quits
 * It takes O(n) pre-processing time and O(1) per edge flip.
 *
 * @param: poly   A polygon triangulation
 */
void flip_and_plot(Poly poly);

/**
 * Verify if a polygon triangulation is valid or not
 *
 * @param poly:  A polygon triangulation
 *
 * @return True if it is a valid polygon triangulation, false otherwise
 */
bool is_valid_poly(const Poly poly);

/**
 * Test poly <-> tree conversion with 3 <= num_of_sides <= TEST_MAX_SIDES_POLY
 * (default 100). There are NUM_OF_TESTS_POLY (default 50k) random test cases for
 * each `num_of_sides`.
 */
void test_conversion_poly();
