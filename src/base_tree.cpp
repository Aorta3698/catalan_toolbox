#include "base_tree.hpp"
#include "global.hpp"
#include "ktree_iter.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numbers>
#include <numeric>
#include <ostream>
#include <stdexcept>

// Read: https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/

std::string BaseTree::to_bitstring() {
  std::string bit;
  std::function<void(const Node *)> pre_order = [&](const Node *cur_node) {
    bit += cur_node->is_internal_node();
    for (const auto &child : cur_node->children) {
      pre_order(child.get());
    }
  };
  pre_order(this->root.get());

  return bit;
}

std::unique_ptr<BaseTree>
BaseTree::get_from_bitstring(const std::string &bitstring) {
  auto [k, n] = Lexi::get_k_n(bitstring);
  std::function<std::unique_ptr<Node>(int)> build = [&](int cur) {
    auto cur_node{std::make_unique<Node>(++cur)};
    if (bitstring[cur] == '0') {
      return cur_node;
    }
    cur_node->children.resize(k);
    for (int i{}; i < k; ++i) {
      cur_node->children[i] = build(cur);
    }
    return cur_node;
  };

  return std::make_unique<BaseTree>(build(0));
}

std::unique_ptr<BaseTree> BaseTree::get_random(int branches, int num_of_nodes) {
  if (num_of_nodes < 1) {
    std::cerr << "internal node count cannot be < 1\n";
    throw std::invalid_argument("");
  }

  // randomly select internal nodes
  int upper{branches * num_of_nodes + 1};
  bool seq[upper];
  std::memset(seq, 0, sizeof(seq));
  std::vector<int> indexes(upper);
  std::iota(indexes.begin(), indexes.end(), 0);
  std::shuffle(indexes.begin(), indexes.end(), g_256ss);
  for (int i{}; i < num_of_nodes; ++i) {
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
  std::function<std::unique_ptr<Node>()> build = [&]() {
    if (++cur == upper) {
      cur = 0;
    }
    auto node{std::make_unique<Node>(cur, seq[cur] ? branches : 0)};
    for (auto &child : node->children) {
      child = build();
    }
    return node;
  };

  return std::make_unique<BaseTree>(build());
}

std::unique_ptr<BaseTree>
BaseTree::get_from_traversal(const std::string &traversal) {
  // convert to int vector
  auto tmp{Util::split_string(traversal, ' ')};
  std::vector<int> vertices{};
  std::transform(tmp.begin(), tmp.end(), std::back_inserter(vertices),
                 [](const auto &s) { return std::stoi(s); });

  // build
  int cur{};
  int sz{int(vertices.size())};
  int leaves{sz};
  std::function<std::unique_ptr<Node>(int, int)> build = [&](int lo, int hi) {
    if (lo > hi) {
      return std::make_unique<Node>(++leaves);
    }
    int root{vertices[cur++]};
    auto cur_node{std::make_unique<Node>(root)};
    cur_node->children.resize(2);
    cur_node->children[0] = build(lo, root - 1);
    cur_node->children[1] = build(root + 1, hi);
    return cur_node;
  };

  return std::make_unique<BaseTree>(build(1, sz), TreeType::Non_Full);
}

void BaseTree::to_file(FileOp mode, std::string file) {
  if (file == "") {
    file = BaseTree::_DEFAULT_PREFIX_FILE;
  }
  auto file_mode =
      mode == FileOp::Truncate ? std::ios_base::trunc : std::ios_base::app;

  assert(this->root);
  std::ofstream out{file, file_mode};
  if (!out) {
    std::cerr << std::format("{} cannot be opened.\n", file);
    throw std::invalid_argument("");
  }
  out << this->type << "\n";

  std::function<void(const Node *)> print_edges = [&](const Node *cur_node) {
    for (const auto &child : cur_node->children) {
      out << cur_node->id << ',' << child->id << "\n";
      print_edges(child.get());
    }
  };

  print_edges(this->root.get());
  // deconstructor for _out_ is auto called
}

std::unique_ptr<BaseTree> BaseTree::get_from_file(std::string file) {
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
    line = Util::strip(line);
    auto tokens{Util::split_string(line, ',')};
    try {
      int v{Util::get_num(tokens[0])};
      int u{Util::get_num(tokens[1])};
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
  std::function<std::unique_ptr<Node>(int, int)> build = [&](int cur_node,
                                                             int parent) {
    auto node{std::make_unique<Node>(cur_node)};
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

  return std::make_unique<BaseTree>(build(root, -1));
}

std::string BaseTree::serialize() {
  std::string encoded_result{};
  std::function<void(const Node *)> encode = [&](const Node *cur_node) {
    int zero{};
    for (const auto &next_node : cur_node->children) {
      zero = encoded_result.size();
      encoded_result += "1";
      encode(next_node.get());
    }
    encoded_result[zero] -= cur_node->is_internal_node();
  };

  encode(root.get());
  return encoded_result;
}

int BaseTree::height() {
  std::function<int(const Node *)> solve = [&](const Node *cur_node) {
    int depth{};
    for (const auto &child : cur_node->children) {
      depth = std::max(depth, solve(child.get()));
    }
    return depth + 1;
  };

  // the book assumes root by itself = 1 height
  return solve(this->root.get());
}

double BaseTree::asymptote(int k, int num_of_internal_nodes) {
  return std::pow(std::numbers::pi * 2.0 * num_of_internal_nodes * k / (k - 1), 0.5);
}

void BaseTree::test_expected_height() {
  int num_of_samples{10'000};
  int num_of_internal_nodes{50'000};
  int ten{num_of_samples / 10};
  double tolerance{0.03};
  std::cout << "====== Random BaseTree: Sampling Distribution Test ======\n\n";
  std::cout << "Testing from degree 2 to 5 with 10k random samples and 50k "
               "internal nodes\n\n";
  for (int deg = 2; deg <= 5; ++deg) {
    double sum{};
    for (int c{}; c < num_of_samples; ++c) {
      if (c / ten * ten == c) {
        std::cout << "\r";
        std::cout << std::format("Deg {} is {}% completed...", deg, c / ten * 10);
        std::cout.flush();
      }
      auto tree{BaseTree::get_random(deg, num_of_internal_nodes)};
      sum += tree->height();
    }
    double res{sum / num_of_samples};
    double expected{BaseTree::asymptote(deg, num_of_internal_nodes)};
    double error{std::abs(res - expected) / expected};

    std::cout << "\r";
    std::cout << std::format(
        "Result: {:<9.2f} | Expected: {:<9.2f} (OK: within 3%, got: {:.2f}%)\n", res,
        expected, error * 100);

    if (error > tolerance) {
      std::cerr << "Error: Exceeded the acceptable tolerance.\n";
      assert(false);
    }
  }

  std::cout << "\n\n";
}
