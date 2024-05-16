#include "global.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <numbers>
#include <numeric>
#include <stdexcept>
#include <unordered_set>

Node *get_random_tree(int branches, int num_nodes) {
  if (num_nodes < 1) {
    std::cerr << "internal node count cannot be < 1\n";
    throw std::invalid_argument("");
  }

  // randomly select internal nodes
  int upper{branches * num_nodes + 1};
  bool seq[upper];
  std::memset(seq, 0, sizeof(seq));
  std::vector<int> indexes(upper);
  std::iota(indexes.begin(), indexes.end(), 0);
  std::shuffle(indexes.begin(), indexes.end(), g_256ss);
  for (int i{}; i < num_nodes; ++i) {
    seq[indexes[i]] = 1;
  }

  // find root
  int score{1};
  int min{2};
  int cur{};
  for (int i{}; i < upper; ++i) {
    score += seq[i] ? branches - 1 : -1;
    if (score < min) {
      min = score;
      cur = i;
    }
  }

  // build tree
  std::function<Node *()> build = [&]() {
    if (++cur == upper) {
      cur = 0;
    }
    auto node{new Node(cur, seq[cur] ? branches : 0)};
    for (auto &child : node->children) {
      child = build();
    }
    return node;
  };

  return build();
}

void store_tree_into_file(const Node *root, std::string file) {
  assert(root);
  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  std::function<void(const Node *)> print_edges = [&](const Node *cur_node) {
    for (const auto child : cur_node->children) {
      out << cur_node->id << ',' << child->id << "\n";
      print_edges(child);
    }
  };

  print_edges(root);
  // deconstructor for _out_ is auto called
}

Node *get_tree_from_file(std::string file) {
  std::ifstream in{file};
  if (!in) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  // build graph adj list
  std::unordered_map<int, std::vector<int>> adj{};
  std::unordered_map<int, int> deg{};
  std::string line{};
  while (getline(in, line)) {
    line = strip(line);
    auto tokens{split_string(line, ',')};
    try {
      int v{get_num(tokens[0])};
      int u{get_num(tokens[1])};
      ++deg[v];
      ++deg[u];
      adj[v].push_back(u);
      adj[u].push_back(v);
    } catch (std::exception &e) {
      std::cerr << "Error: Data in file is badly formatted.\n";
      throw std::invalid_argument("");
    }
  }

  // find root - assuming data is sound
  int root{};
  int min = 1e9;
  for (const auto [id, count] : deg) {
    if (count < min) {
      min = count;
      root = min;
    } else if (count < root) {
      root = min;
    }
  }

  // build tree
  int branches{deg[root]};
  std::function<Node *(int, int)> build = [&](int cur_node, int parent) {
    Node *node{new Node(cur_node)};
    for (int child : adj[cur_node]) {
      if (child != parent) {
        node->children.push_back(build(child, cur_node));
      }
    }
    if (int count = node->children.size(); count && count != branches) {
      std::cerr << "Error: This is not a tree formed by a Dyck Path\n";
      throw std::invalid_argument("");
    }
    return node;
  };

  return build(root, -1);
}

void plot_tree(std::string file) { plot(TREE_PLOT_SCRIPT, file); }

void plot_all_trees(int num_of_internal_nodes) {
  if (num_of_internal_nodes < 1 || num_of_internal_nodes > 4) {
    std::cerr << "can't plot that many!\n";
    throw std::invalid_argument("");
  }

  std::unordered_set<std::string> seen{};
  int sz{get_catalan(num_of_internal_nodes)};
  while (int(seen.size()) < sz) {
    auto tree{get_random_tree(2, num_of_internal_nodes)};
    std::string id{serialize_tree(tree)};
    if (!seen.contains(id)) {
      std::string file{".tree" + std::to_string(seen.size())};
      store_tree_into_file(tree, file);
      plot_tree(file);
      seen.insert(id);
    }
    free_tree(tree);
  }
}

std::string serialize_tree(const Node *root) {
  std::string encoded_result{};
  std::function<void(const Node *)> encode = [&](const Node *cur_node) {
    int zero{};
    for (const auto next_node : cur_node->children) {
      zero = encoded_result.size();
      encoded_result += "1";
      encode(next_node);
    }
    encoded_result[zero] -= cur_node->is_internal_node();
  };

  encode(root);
  return encoded_result;
}

void free_tree(Node *cur_node) {
  for (auto child : cur_node->children) {
    free_tree(child);
  }
  delete cur_node;
}

int height(const Node *root) {
  std::function<int(const Node *)> solve = [&](const Node *cur_node) {
    int depth{};
    for (const auto &child : cur_node->children) {
      depth = std::max(depth, solve(child));
    }
    return depth + 1;
  };

  // the book assumes root by itself = 1 height
  return solve(root);
}

double asymptote(int k, int num_of_internal_nodes) {
  return std::pow(std::numbers::pi * 2.0 * num_of_internal_nodes * k / (k - 1), 0.5);
}
