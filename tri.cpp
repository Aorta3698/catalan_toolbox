#include "tri.hpp"
#include "util.hpp"

#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <thread>
#include <unordered_set>

Poly tree_to_poly(const Node *root) {
  Poly lines;

  int count{};
  enum Dir { Left, Right, None };

  std::function<int(const Node *, Dir)> build = [&](const Node *cur_node, Dir dir) {
    if (cur_node->is_leaf()) {
      return ++count - (Dir::Left == dir);
    }
    assert(int(cur_node->children.size()) == 2);
    int l{build(cur_node->children[0], Dir::Left)};
    int r{build(cur_node->children[1], Dir::Right)};
    lines.push_back({l, r});
    return dir == Dir::Left ? l : r;
  };

  build(root, Dir::None);
  return lines;
}

// TODO: extracting plot into util.cpp
// TODO: look into:
// https://stackoverflow.com/questions/46784028/edge-length-in-networkx
void plot_poly(Poly poly, std::string file) {
  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  int num_of_sides{int(poly.size()) + 2};
  // points and coordinates
  out << num_of_sides << "\n";
  double x{};
  double y{1.0};
  for (int i{}; i < num_of_sides; ++i) {
    out << std::fixed << std::setprecision(3) << x << "," << y << "\n";
    auto [nx, ny] = rotate(x, y, 360.0 / num_of_sides);
    x = nx;
    y = ny;
  }

  // edges
  for (int i{}; i < num_of_sides - 1; ++i) {
    out << i << "," << i + 1 << "\n";
  }
  for (auto [a, b] : poly | std::views::reverse) {
    out << a << "," << b << "\n";
  }

  if (fork() == 0) { // TODO: calling fork inside thread is not good
    std::string python{"./visualizer/bin/python3 "};
    std::string script{"./visualizer/plot-poly.py "};
    std::string cmd{python + script + file};
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}

void plot_all_poly(int num_of_sides) {
  if (num_of_sides < 3 || num_of_sides > 6) {
    std::cerr << "can't plot that many!\n";
    throw std::invalid_argument("");
  }

  std::unordered_set<std::string> seen;
  int total{get_catalan(num_of_sides - 2)};
  while (int(seen.size()) < total) {
    auto tree{get_random_tree(2, num_of_sides - 2)};
    std::string id{serialize_tree(tree)};
    if (!seen.contains(id)) {
      seen.insert(id);
      std::string file{".p" + std::to_string(seen.size())};
      Poly poly{tree_to_poly(tree)};
      plot_poly(poly, file);
    }
    free_tree(tree);
  }
}
