#include "global.hpp"
#include "tree.hpp"
#include "tri.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

Poly tree_to_poly(const Node *root) {
  Poly poly;

  int count{};
  enum Dir { Left, Right, None };

  std::function<int(const Node *, Dir)> build = [&](const Node *cur_node, Dir dir) {
    if (cur_node->is_leaf()) {
      return ++count - (Dir::Left == dir);
    }
    assert(int(cur_node->children.size()) == 2);
    int l{build(cur_node->children[0], Dir::Left)};
    int r{build(cur_node->children[1], Dir::Right)};
    poly.push_back({l, r});
    return dir == Dir::Left ? l : r;
  };

  build(root, Dir::None);
  std::ranges::reverse(poly);
  return poly;
}

Node *poly_to_tree(const Poly poly) {
  enum Dir { Up, Down };
  int num_of_sides = int(poly.size() + 2);
  int tables[num_of_sides][2];
  std::memset(tables, 0, sizeof(tables));
  std::vector<Node *> nodes(num_of_sides);
  std::vector<std::vector<int>> buckets(num_of_sides);

  // pre-processing
  for (int i{}; i < num_of_sides - 1; ++i) {
    nodes[i] = new Node(i);
    tables[i][Dir::Up] = i;
    tables[i + 1][Dir::Down] = i;
  }
  for (int i{}; i < int(poly.size()); ++i) {
    auto [l, r] = poly[i];
    assert(r > l);
    buckets[r - l].push_back(i);
  }

  // build tree
  int id{num_of_sides - 2};
  for (int len{}; len < num_of_sides; ++len) {
    for (int idx : buckets[len]) {
      auto [l, r] = poly[idx];
      auto parent = new Node(++id, 2);
      parent->children[0] = nodes[tables[l][Dir::Up]];
      parent->children[1] = nodes[tables[r][Dir::Down]];
      int pos{tables[l][Dir::Up]};
      nodes[pos] = parent;
      tables[r][Dir::Down] = pos;
      tables[l][Dir::Up] = pos;
    }
  }

  return nodes[tables[0][Dir::Up]];
}

Poly flip_an_edge(Poly poly, int idx) {}

Poly get_random_poly(int num_of_sides) {
  auto tree{get_random_tree(2, num_of_sides - 2)};
  Poly res{tree_to_poly(tree)};
  free_tree(tree);
  return res;
}

void plot_random_poly(int num_of_sides, int count) {
  while (count--) {
    auto poly{get_random_poly(num_of_sides)};
    plot_poly(poly, ".poly" + std::to_string(count));
  }
}

void plot_poly(Poly poly, std::string file) {
  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  int num_of_sides{int(poly.size()) + 2};

  // points and coordinates
  out << num_of_sides << "\n";
  auto [x, y] = rotate(0.0, 1.0, 180.0 / num_of_sides);
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
  for (auto [a, b] : poly) {
    out << a << "," << b << "\n";
  }

  plot(POLY_PLOT_SCRIPT, file);
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

void test_conversion_poly() {
  std::cout << "Starting testing conversion between poly and tree with num_of_sides "
               "from 3 to "
            << TEST_MAX_SIDES_POLY << "\n\n";

  for (int num_of_sides{3}; num_of_sides <= TEST_MAX_SIDES_POLY; ++num_of_sides) {
    for (int i{}; i < NUM_OF_TESTS_POLY; ++i) {
      auto tree1{get_random_tree(2, num_of_sides - 2)};
      auto tree2{poly_to_tree(tree_to_poly(tree1))};
      std::string id1{serialize_tree(tree1)};
      std::string id2{serialize_tree(tree2)};
      if (id1 != id2) {
        std::cerr << "Test Failed:\n"
                  << "num_of_sides = " << num_of_sides << "\n"
                  << "id1 = " << id1 << "\n"
                  << "id2 = " << id2 << "\n";
        assert(false);
      }
      free_tree(tree1);
      free_tree(tree2);
    }
    std::cout << "num_of_sides = " << num_of_sides << " done for "
              << NUM_OF_TESTS_POLY << " random tests!\n";
  }
}
