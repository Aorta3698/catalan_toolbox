#include "arcs.hpp"
#include "chords.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

Arcs tree_to_arcs(const Node *root) {
  return tree_to_chords(root);
  // Arcs arcs;
  // int id{};

  // std::function<int(const Node *)> build = [&](const Node *cur_node) {
  //   if (cur_node->is_leaf()) {
  //     return id++;
  //   }
  //   assert(cur_node->child_count() == 2);
  //   int assigned_id{id++};
  //   int left_id{build(cur_node->children[0])};
  //   int right_id{build(cur_node->children[1])};
  //   arcs.push_back({left_id, right_id});
  //   return assigned_id;
  // };

  // build(root);
  // return arcs;
}

Node *arcs_to_tree(const Arcs arcs) {
  return chords_to_tree(arcs);
  // int num_of_nodes = int(arcs.size()) << 1;
  // int portal[num_of_nodes];
  // int id{};
  // for (const auto &[l, r] : arcs) {
  //   assert(r > l);
  //   portal[l] = r;
  // }

  // std::function<Node *(int, int)> build = [&](int lo, int hi) {
  //   if (lo >= hi) {
  //     return new Node(++id);
  //   }
  //   auto cur_node{new Node(++id)};
  //   ++lo;
  //   cur_node->children.push_back(build(lo, portal[lo] - 1));
  //   cur_node->children.push_back(build(portal[lo], hi));
  //   return cur_node;
  // };

  // return build(0, num_of_nodes);
}

Poly get_random_arcs(int num_of_points) {
  return get_random_chords(num_of_points);
  // if (num_of_points & 1) {
  //   std::cerr << "Error: number of points must be even\n";
  //   throw std::invalid_argument("");
  // }
  // auto tree{get_random_tree(2, num_of_points >> 1)};
  // Poly res{tree_to_arcs(tree)};
  // free_tree(tree);
  // return res;
}

void plot_random_arcs(int num_of_points, int count) {
  if (num_of_points & 1) {
    std::cerr << "Error: number of points must be even\n";
    throw std::invalid_argument("");
  }
  while (count--) {
    auto arcs{get_random_arcs(num_of_points)};
    plot_arcs(arcs, ".arcs" + std::to_string(count));
  }
}

void plot_arcs(Arcs arcs, std::string file) {
  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  int num_of_points{int(arcs.size()) << 1};

  // points and coordinates
  out << num_of_points << "\n";
  for (int i{}; i < num_of_points; ++i) {
    out << 0.3 * i << "," << 0 << "\n";
  }

  // edges
  for (auto [a, b] : arcs) {
    out << a << "," << b << "\n";
  }

  plot(ARCS_PLOT_SCRIPT, file);
}

void plot_all_arcs(int num_of_points) {
  if (num_of_points & 1) {
    std::cerr << "Error: number of points must be even\n";
    throw std::invalid_argument("");
  }

  int cat{num_of_points >> 1};
  if (cat < 0 || cat > 4) {
    std::cerr << "can't plot that many!\n";
    throw std::invalid_argument("");
  }

  std::unordered_set<std::string> seen;
  int total{get_catalan(cat)};
  while (int(seen.size()) < total) {
    auto tree{get_random_tree(2, cat)};
    std::string id{serialize_tree(tree)};
    if (!seen.contains(id)) {
      std::string file{".arcs" + std::to_string(seen.size())};
      Arcs arcs{tree_to_arcs(tree)};
      plot_arcs(arcs, file);
      seen.insert(id);
    }
    free_tree(tree);
  }
}

void exchage_arcs(Arcs arcs) {
  // TODO
  assert(false);
}

bool is_valid_arcs(const Arcs arcs) {
  // TODO
  assert(false);
}

void test_conversion_arcs() {
  std::cout << "Starting testing conversion between arcs and tree with total points "
               "from 2 to "
            << TEST_MAX_SIDES_ARCS << " with 2 increment\n\n";

  for (int num_of_points{2}; num_of_points <= TEST_MAX_SIDES_ARCS;
       num_of_points += 2) {
    for (int i{}; i < NUM_OF_TESTS_ARCS; ++i) {
      auto tree1{get_random_tree(2, num_of_points >> 1)};
      auto tree2{arcs_to_tree(tree_to_arcs(tree1))};
      std::string id1{serialize_tree(tree1)};
      std::string id2{serialize_tree(tree2)};
      if (id1 != id2) {
        std::cerr << "Test Failed:\n"
                  << "Total Points = " << num_of_points << "\n"
                  << "id1 = " << id1 << "\n"
                  << "id2 = " << id2 << "\n";
        assert(false);
      }
      free_tree(tree1);
      free_tree(tree2);
    }
    std::cout << "Total points = " << num_of_points << " done for "
              << NUM_OF_TESTS_ARCS << " random tests!\n";
  }

  std::cout << "\n\nAll Tests Completed!\n\n";
}
