#pragma once

#include "tree.hpp"
#include <array>
#include <string>
#include <vector>

using Edge = std::array<int, 2>;
using Poly = std::vector<Edge>;

Poly tree_to_poly(const Node *root);

Node *poly_to_tree(const Poly poly);

void plot_poly(Poly poly, std::string file);

void plot_all_poly(int num_of_sides);

Poly get_random_poly(int num_of_sides);

void plot_random_poly(int num_of_sides, int count);
