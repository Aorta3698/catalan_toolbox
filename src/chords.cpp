#include "chords.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

std::unique_ptr<Chords> Chords::of(std::unique_ptr<BaseTree> base_tree) {
  auto tree{Tree::of(std::move(base_tree))};
  return tree->to_chords();
}

std::unique_ptr<Tree> Chords::to_tree() {
  if (!Chords::is_valid(this->chords)) {
    std::cerr << "Error: not a valid chords graph\n";
    throw std::invalid_argument("");
  }
  std::vector<std::unique_ptr<Node>> nodes(this->points + 1);
  std::vector<Node *> parents(this->points);
  for (const auto &[l, r] : chords) {
    assert(r > l);
    nodes[l] = std::make_unique<Node>(l, 2);
    nodes[r] = std::make_unique<Node>(r); // leaf
    parents[l] = nodes[l].get();
  }
  nodes[this->points] = std::make_unique<Node>(this->points); // leaf

  for (const auto &[l, r] : chords) {
    parents[l]->children[0] = std::move(nodes[l + 1]);
    parents[l]->children[1] = std::move(nodes[r + 1]);
  }

  return std::make_unique<Tree>(std::move(nodes[0]));
}

std::unique_ptr<Arcs> Chords::to_arcs() {
  return std::make_unique<Arcs>(this->chords);
}

std::unique_ptr<Chords> Chords::get_random(int num_of_points) {
  if (num_of_points & 1) {
    std::cerr << "Error: number of points must be even\n";
    throw std::invalid_argument("");
  }
  auto tree{Tree::get_random(2, num_of_points >> 1)};
  auto res{tree->to_chords()};
  return res;
}

std::unique_ptr<Chords> Chords::next() {
  // TODO
  assert(false);
}
void Chords::to_file(FileOp mode, std::string file) {
  if (file == "") {
    file = Chords::_DEFAULT_PREFIX_FILE;
  }
  auto file_mode =
      mode == FileOp::Truncate ? std::ios_base::trunc : std::ios_base::app;

  std::ofstream out{file, file_mode};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  // points and coordinates
  out << this->points << "\n";
  auto [x, y] = Util::rotate(0.0, 1.0, 180.0 / this->points);
  for (int i{}; i < this->points; ++i) {
    out << std::format("{:.3f},{:.3f}\n", x, y);
    auto [nx, ny] = Util::rotate(x, y, 360.0 / this->points);
    x = nx;
    y = ny;
  }

  for (auto [a, b] : chords) {
    out << a << "," << b << "\n";
  }
}

void Chords::exchage_chords() {
  // TODO
  assert(false);
}

bool Chords::is_valid(const Graph &graph) {
  int points{int(graph.size()) << 1};
  std::vector<int> portal(points, -1);
  for (const auto &[l, r] : graph) {
    if (portal[l] >= 0 || portal[r] >= 0) { // a point in two chords
      return false;
    }
    portal[l] = r;
    portal[r] = l;
  }
  std::vector<int> stack;
  for (int i{}; i < points; ++i) {
    if (portal[i] > i) {
      stack.push_back(i);
    } else if (stack.empty() || stack.back() != portal[i]) { // two chords cross
      return false;
    } else {
      stack.pop_back();
    }
  }
  return true;
}

void Chords::test_conversion() {
  std::cout << "====== Chords Graph: Conversion Test ======\n\n";
  std::cout
      << "Starting testing conversion between chords and tree with total points "
         "from 2 to "
      << Chords::_TEST_MAX_SIDES << " with 2 increment\n\n";

  for (int num_of_points{2}; num_of_points <= Chords::_TEST_MAX_SIDES;
       num_of_points += 2) {
    for (int i{}; i < Chords::_NUM_OF_TESTS; ++i) {
      auto tree1{Tree::get_random(2, num_of_points >> 1)};
      auto tree2{tree1->to_chords()->to_tree()};
      std::string id1{tree1->serialize()};
      std::string id2{tree2->serialize()};
      if (id1 != id2) {
        std::cerr << "Test Failed:\n";
        std::cerr << std::format("points = {}\n", num_of_points);
        std::cerr << std::format("id1 = {}\n", id1);
        std::cerr << std::format("id2 = {}\n", id2);
        assert(false);
      }
    }
    std::cout << std::format("Total points = {:3} done for {} random tests!\n",
                             num_of_points, Chords::_NUM_OF_TESTS);
  }

  std::cout << "\n\nAll Tests Completed!\n\n";
}
