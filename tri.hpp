#include "tree.hpp"
#include <array>
#include <string>
#include <vector>

using Poly = std::vector<std::array<int, 2>>;

Poly tree_to_poly(const Node *root);
void plot_poly(Poly poly, std::string file);
void plot_all_poly(int num_of_sides);
