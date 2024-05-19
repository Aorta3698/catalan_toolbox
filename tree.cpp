#include "coin.hpp"
#include "global.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <numeric>
#include <ranges>
#include <stdexcept>

// TODO: this doesn't seem ideal (converting 2 times)
CoinStack *Tree::to_coin_stack() { return this->to_dyck_pre()->into_coin_stack(); }

CoinStack *Tree::into_coin_stack() {
  auto coins{this->to_dyck_pre()->into_coin_stack()};
  this->self_destruct();
  return coins;
}

DyckPreMirrored *Tree::to_dyck_pre_mirrored() {
  std::string encoded_result{};
  std::function<void(const Node *)> encode = [&](const Node *cur_node) {
    int zero{int(encoded_result.size())};
    for (const auto next_node : cur_node->children | std::ranges::views::reverse) {
      encoded_result += "1";
      encode(next_node);
    }
    if (cur_node->is_internal_node()) {
      encoded_result[zero] = '0';
    }
  };

  encode(root);
  if (!DyckPreMirrored::is_valid(encoded_result)) {
    std::cerr << "Error: This tree is not a full k-ary tree\n";
    throw std::invalid_argument("");
  }
  return new DyckPreMirrored(encoded_result);
}

DyckPreMirrored *Tree::into_dyck_pre_mirrored() {
  auto dyck_path{this->to_dyck_pre_mirrored()};
  this->self_destruct();
  return dyck_path;
}

DyckPre *Tree::to_dyck_pre() {
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
  if (!DyckPre::is_valid(encoded_result)) {
    std::cerr << "Error: This tree is not a full k-ary tree\n";
    throw std::invalid_argument("");
  }
  return new DyckPre(encoded_result);
}

DyckPre *Tree::into_dyck_pre() {
  auto dyck_pre{this->to_dyck_pre()};
  this->self_destruct();
  return dyck_pre;
}

Arcs *Tree::to_arcs() {
  auto chords{this->to_chords()};
  auto arcs{chords->into_arcs()};
  return arcs;
}

Arcs *Tree::into_arcs() {
  auto arcs{this->to_arcs()};
  this->self_destruct();
  return arcs;
}

Chords *Tree::to_chords() {
  Graph chords;
  int id{};

  std::function<int(const Node *)> build = [&](const Node *cur_node) {
    if (cur_node->is_leaf()) {
      return id++;
    }
    assert(cur_node->child_count() == 2);
    int assigned_id{id++};
    int left_id{build(cur_node->children[0])};
    int right_id{build(cur_node->children[1])};
    chords.push_back({left_id, right_id});
    return assigned_id;
  };

  build(this->root);
  if (!Chords::is_valid(chords)) {
    std::cerr << "Error: tree to chords graph failed.\n";
    throw std::invalid_argument("");
  }
  return new Chords(chords);
}

Chords *Tree::into_chords() {
  auto chords{this->to_chords()};
  this->self_destruct();
  return chords;
}

Poly *Tree::to_poly() {
  Graph poly;

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

  build(this->root, Dir::None);
  std::ranges::reverse(poly);
  if (!Poly::is_valid(poly)) {
    std::cerr << "Error: tree to polygon triangulation failed.\n";
    throw std::invalid_argument("");
  }
  return new Poly(poly);
}

Poly *Tree::into_poly() {
  auto poly{this->to_poly()};
  this->self_destruct();
  return poly;
}

Tree *Tree::get_random(int branches, int num_of_nodes) {
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

  return new Tree(build());
}

void Tree::store_into_file(std::string file) {
  assert(this->root);
  std::ofstream out{file};
  if (!out) {
    std::cerr << std::format("{} cannot be opened.\n", file);
    throw std::invalid_argument("");
  }

  std::function<void(const Node *)> print_edges = [&](const Node *cur_node) {
    for (const auto child : cur_node->children) {
      out << cur_node->id << ',' << child->id << "\n";
      print_edges(child);
    }
  };

  print_edges(this->root);
  // deconstructor for _out_ is auto called
}

Tree *Tree::get_from_file(std::string file) {
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

  return new Tree(build(root, -1));
}

void Tree::plot(std::string file) {
  if (file == "") {
    file = Tree::_DEFAULT_FILE_PREFIX;
  }
  Util::plot(Tree::_PLOT_SCRIPT, file);
}

std::string Tree::serialize() {
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

void Tree::self_destruct() {
  std::function<void(Node *)> free_nodes = [&](Node *cur_node) {
    for (auto child : cur_node->children) {
      free_nodes(child);
    }
    delete cur_node;
  };
  free_nodes(this->root);
  delete this;
}

int Tree::height() {
  std::function<int(const Node *)> solve = [&](const Node *cur_node) {
    int depth{};
    for (const auto &child : cur_node->children) {
      depth = std::max(depth, solve(child));
    }
    return depth + 1;
  };

  // the book assumes root by itself = 1 height
  return solve(this->root);
}

double Tree::asymptote(int k, int num_of_internal_nodes) {
  return std::pow(std::numbers::pi * 2.0 * num_of_internal_nodes * k / (k - 1), 0.5);
}

void Tree::test_expected_height() {
  int num_of_samples{10'000};
  int num_of_internal_nodes{100'000};
  int ten{num_of_samples / 10};
  double tolerance{0.02};
  std::cout << "====== Sampling Distribution Test ======\n\n";
  std::cout << "Testing from degree 2 to 5 with 10k random samples and 100k "
               "internal nodes\n\n";
  for (int deg = 2; deg <= 5; ++deg) {
    double sum{};
    for (int c{}; c < num_of_samples; ++c) {
      if (c / ten * ten == c) {
        std::cout << "\r";
        std::cout << "Deg " << deg << " is " << c / ten * 10 << "% completed...";
        std::cout.flush();
      }
      auto tree{Tree::get_random(deg, num_of_internal_nodes)};
      sum += tree->height();
      tree->self_destruct();
    }
    double res{sum / num_of_samples};
    double expected{Tree::asymptote(deg, num_of_internal_nodes)};
    double error{std::abs(res - expected) / expected};
    std::cout << "\r";
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << "Result: " << res << "  |  Expected: " << expected;
    std::cout << " (Allowed Tolerance: 2.00%, got: " << error * 100 << "%)\n";
    if (error > tolerance) {
      std::cerr << "Error: Exceeded the acceptable tolerance.\n";
      assert(false);
    }
  }

  std::cout << "\n\n";
}
