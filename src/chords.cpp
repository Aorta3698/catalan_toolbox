#include "chords.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

std::unique_ptr<Chords> Chords::of(const std::string &mtree) {
  auto tree{Tree::get_from_traversal(mtree)};
  return tree->to_chords();
}

std::unique_ptr<Tree> Chords::to_tree() {
  int portal[this->points];
  int id{};
  for (const auto &[l, r] : chords) {
    assert(r > l);
    portal[l] = r;
  }

  std::function<std::unique_ptr<Node>(int, int)> build = [&](int lo, int hi) {
    auto cur_node{std::make_unique<Node>(++id)};
    if (lo >= hi) {
      return cur_node;
    }
    ++lo;
    cur_node->children.push_back(build(lo, portal[lo] - 1));
    cur_node->children.push_back(build(portal[lo], hi));
    return cur_node;
  };

  return std::make_unique<Tree>(build(0, this->points));
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

void Chords::to_file(std::string file) {
  if (file == "") {
    file = Chords::_DEFAULT_PREFIX_FILE;
  }

  std::ofstream out{file};
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
  // TODO
  // assert(false);
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
