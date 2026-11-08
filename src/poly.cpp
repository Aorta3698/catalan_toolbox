#include "poly.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_set>

std::unique_ptr<Poly> Poly::of(std::unique_ptr<BaseTree> base_tree) {
  auto tree{Tree::of(std::move(base_tree))};
  return tree->to_poly();
}

std::unique_ptr<Tree> Poly::to_tree() {
  enum Dir { Up, Down };
  int tables[this->sides][2];
  std::memset(tables, 0, sizeof(tables));
  std::vector<std::unique_ptr<Node>> nodes(this->sides);
  std::vector<std::vector<int>> buckets(this->sides);

  // pre-processing
  for (int i{}; i < this->sides - 1; ++i) {
    nodes[i] = std::make_unique<Node>(i);
    tables[i][Dir::Up] = i;
    tables[i + 1][Dir::Down] = i;
  }
  for (int i{}; i < int(poly.size()); ++i) {
    auto [l, r] = poly[i];
    assert(r > l);
    buckets[r - l].push_back(i);
  }

  // build tree
  int id{this->sides - 2};
  for (int len{}; len < this->sides; ++len) {
    for (int idx : buckets[len]) {
      auto [l, r] = poly[idx];
      auto parent = std::make_unique<Node>(++id, 2);
      parent->children[0] = std::move(nodes[tables[l][Dir::Up]]);
      parent->children[1] = std::move(nodes[tables[r][Dir::Down]]);
      int pos{tables[l][Dir::Up]};
      nodes[pos] = std::move(parent);
      tables[r][Dir::Down] = pos;
      tables[l][Dir::Up] = pos;
    }
  }

  return std::make_unique<Tree>(std::move(nodes[tables[0][Dir::Up]]));
}

std::unique_ptr<Bracketing> Poly::to_bracketing() {
  std::vector<int> start(this->sides);
  std::vector<int> end(this->sides);
  for (const auto &[l, r] : this->poly) {
    assert(r > l);
    ++start[l];
    ++end[r];
  }

  // walk the cut corners
  std::string res;
  for (int i{}; i < this->sides; ++i) {
    res += std::string(end[i], ')') + std::string(start[i], '(');
    if (i < this->sides - 1) {
      res += "x";
    }
  }

  // drop the root side's pair
  return std::make_unique<Bracketing>(res.substr(1, res.size() - 2));
}

void Poly::flip_and_plot() {
  // pre-processing - O(n)
  enum Relation { Left, Right, Parent }; // left, right, parent
  int sz{int(this->poly.size())};
  int relationship[sz + this->sides][3];
  std::memset(relationship, 0, sizeof(relationship));
  std::vector<int> slot(this->sides);
  std::vector<int> pred(this->sides);
  std::vector<std::vector<int>> buckets(this->sides);
  std::function<Edge(int)> get_edge = [&](int idx) {
    return idx < sz ? this->poly[idx] : std::array<int, 2>{{idx - sz, idx - sz + 1}};
  };
  for (int i{}; i < this->sides - 1; ++i) {
    slot[i] = sz + i;
    pred[i + 1] = i;
  }
  for (int i{}; i < int(this->poly.size()); ++i) {
    auto [l, r] = this->poly[i];
    assert(r > l);
    buckets[r - l].push_back(i);
  }
  for (int len{}; len < this->sides; ++len) {
    for (int idx : buckets[len]) {
      auto [l, r] = poly[idx];
      int m{pred[r]};
      relationship[idx][Relation::Left] = slot[l];
      relationship[idx][Relation::Right] = slot[m];
      relationship[slot[l]][Relation::Parent] = idx;
      relationship[slot[m]][Relation::Parent] = idx;
      slot[l] = idx;
      pred[r] = l;
    }
  }

  std::function<std::unique_ptr<Node>(int)> subtree = [&](int i) {
    if (i >= sz) {
      return std::make_unique<Node>(i);
    }
    auto node{std::make_unique<Node>(i, 2)};
    node->children[0] = subtree(relationship[i][Relation::Left]);
    node->children[1] = subtree(relationship[i][Relation::Right]);
    return node;
  };
  auto save = [&]() {
    this->to_file();
    OsOp::write_to_file(Poly::_DEFAULT_PREFIX_FILE, "tree\n", FileOp::Append);
    Tree{subtree(0)}.to_file(FileOp::Append, Poly::_DEFAULT_PREFIX_FILE);
  };

  // plot it to user
  save();
  std::string window{std::format("./visualizer/bin/python3 ./visualizer/{} {}",
                                 Poly::_PLOT_SCRIPT, Poly::_DEFAULT_PREFIX_FILE)};
  FILE *clicks{popen(window.c_str(), "r")};
  if (!clicks) {
    std::cerr << "The flip window could not be started.\n";
    return;
  }
  std::cout << "Click a diagonal to flip it. Close the window to return.\n";

  // flip an edge - O(1)
  while (1) {
    int idx{-1};
    while (idx < 1 || idx >= sz) {
      if (std::fscanf(clicks, "%d", &idx) != 1) {
        pclose(clicks);
        return;
      }
    }

    // locate the other 2 vertices
    int parent{relationship[idx][Relation::Parent]};
    auto [l, r] = this->poly[idx];
    auto [p_l, p_r] = get_edge(parent);
    int u = get_edge(relationship[idx][Relation::Left])[1];
    int v = p_l != l && p_l != r ? p_l : p_r;
    if (u > v) { // make u < v
      std::swap(u, v);
    }

    // bookkeeping stuff
    int is_left_child = relationship[parent][Relation::Left] == idx;
    int sibling{is_left_child ? relationship[parent][Relation::Right]
                              : relationship[parent][Relation::Left]};
    int opposite{!is_left_child}; // left child remains a child or not
    int tmp{relationship[idx][Relation::Left ^ opposite]};
    relationship[idx][Relation::Left ^ opposite] =
        relationship[idx][Relation::Right ^ opposite];
    relationship[idx][Relation::Right ^ opposite] = sibling;
    relationship[sibling][Relation::Parent] = idx;
    relationship[tmp][Relation::Parent] = parent;
    relationship[parent][Relation::Left] = is_left_child ? tmp : idx;
    relationship[parent][Relation::Right] = is_left_child ? idx : tmp;

    // update graph
    this->poly[idx] = {{u, v}};
    save();
    std::ofstream out{Poly::_WATCHDOG_FILE};
    if (!out) {
      std::cerr << std::format("{} cannot be opened.\n", Poly::_WATCHDOG_FILE);
      throw std::invalid_argument("");
    }
    out << idx << "\n";
    out << u << "," << v << "\n";
  }
}

std::unique_ptr<Poly> Poly::get_random(int num_of_sides) {
  auto tree{Tree::get_random(2, num_of_sides - 2)};
  auto res{tree->to_poly()};
  return res;
}

void Poly::to_file(FileOp mode, std::string file) {
  if (file == "") {
    file = Poly::_DEFAULT_PREFIX_FILE;
  }
  auto file_mode =
      mode == FileOp::Truncate ? std::ios_base::trunc : std::ios_base::app;

  std::ofstream out{file, file_mode};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  // points and coordinates
  out << this->sides << "\n";
  auto [x, y] = Util::rotate(0.0, 1.0, 180.0 / this->sides);
  for (int i{}; i < this->sides; ++i) {
    out << std::format("{:.3f},{:.3f}\n", x, y);
    auto [nx, ny] = Util::rotate(x, y, 360.0 / this->sides);
    x = nx;
    y = ny;
  }

  // edges
  for (int i{}; i < this->sides - 1; ++i) {
    out << i << "," << i + 1 << "\n";
  }
  for (auto [a, b] : this->poly) {
    out << a << "," << b << "\n";
  }
}

std::unique_ptr<Poly> Poly::next() {
  // TODO: implement this
  assert(false);
}

bool Poly::is_valid(const Graph &poly) {
  int num_of_sides{int(poly.size()) + 2};
  int end[num_of_sides];
  int start[num_of_sides];
  std::memset(end, 0, sizeof(end));
  std::memset(start, 0, sizeof(start));
  std::vector<std::unordered_set<int>> lines(num_of_sides);
  std::vector<int> stack{-1};

  for (const auto &edge : poly) {
    auto [s, e] = edge;
    lines[s].insert(e);
    ++end[e];
    ++start[s];
  }
  for (int i{}; i < num_of_sides; ++i) {
    while (end[i]-- && stack.back() >= 0) {
      int start{stack.back()};
      stack.pop_back();
      if (!lines[start].erase(i)) {
        return false;
      }
    }
    while (start[i]--) {
      stack.push_back(i);
    }
  }
  return true;
}

void Poly::test_conversion() {
  std::cout << "====== Polygon Triangulation: Conversion Test ======\n\n";
  std::cout << "Starting testing conversion between poly and tree with num_of_sides "
               "from 3 to "
            << Poly::_TEST_MAX_SIDES << "\n\n";

  for (int num_of_sides{3}; num_of_sides <= Poly::_TEST_MAX_SIDES; ++num_of_sides) {
    for (int i{}; i < Poly::_NUM_OF_TESTS; ++i) {
      auto tree1{Tree::get_random(2, num_of_sides - 2)};
      auto tree2{tree1->to_poly()->to_tree()};
      std::string id1{tree1->serialize()};
      std::string id2{tree2->serialize()};
      if (id1 != id2) {
        std::cerr << "Test Failed:\n";
        std::cerr << std::format("sides = {}\n", num_of_sides);
        std::cerr << std::format("id1 = {}\n", id1);
        std::cerr << std::format("id2 = {}\n", id2);
        assert(false);
      }
    }
    std::cout << std::format("Total sides = {:3} done for {} random tests!\n",
                             num_of_sides, Poly::_NUM_OF_TESTS);
  }

  std::cout << "\n\nAll Tests Completed!\n\n";
}
