#include "global.hpp"
#include "tree.hpp"

Chords tree_to_chords(Node *root);

Node *chords_to_tree(Chords chords);

void plot_chords(Chords chords, std::string file);

void plot_all_chords(int num_of_points);

Poly get_random_chords(int num_of_points);

void plot_random_chords(int num_of_points, int count);

void exchage_chords(Chords chords);

void test_conversion_chords();
