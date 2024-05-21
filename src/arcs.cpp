#include "arcs.hpp"
#include "chords.hpp"
#include "tree.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

std::unique_ptr<Arcs> Arcs::of(const Mutze::Tree mtree) {
  auto tree{Tree::get_from_Mutze(mtree)};
  return tree->to_arcs();
}

std::unique_ptr<Chords> Arcs::to_chords() {
  return std::make_unique<Chords>(this->arcs);
}

std::unique_ptr<Tree> Arcs::to_tree() {
  auto chords(this->to_chords());
  return chords->to_tree();
}

std::unique_ptr<Arcs> Arcs::get_random(int num_of_points) {
  return Chords::get_random(num_of_points)->to_arcs();
}

void Arcs::to_file(std::string file) {
  if (file == "") {
    file = Arcs::_DEFAULT_PREFIX_FILE;
  }

  std::ofstream out{file};
  if (!out) {
    std::cerr << std::format("{} cannot be opened.\n", file);
    throw std::invalid_argument("");
  }

  // points and coordinates
  out << this->points << "\n";
  for (int i{}; i < this->points; ++i) {
    out << 0.3 * i << "," << 0 << "\n";
  }

  // edges
  for (auto [a, b] : arcs) {
    out << a << "," << b << "\n";
  }
}

std::unique_ptr<Arcs> Arcs::next() {
  // TODO
  assert(false);
}

void Arcs::exchage_arcs() {
  // TODO
  assert(false);
}

bool Arcs::is_valid(const Graph &graph) {
  // TODO
  // assert(false);
  return true;
}

void Arcs::test_conversion() {
  std::cout << "====== Arcs Graph: Conversion Test ======\n\n";
  std::cout << "Starting testing conversion between arcs and tree with total points "
               "from 2 to "
            << Arcs::_TEST_MAX_SIDES << " with 2 increment\n\n";

  for (int num_of_points{2}; num_of_points <= Arcs::_TEST_MAX_SIDES;
       num_of_points += 2) {
    for (int i{}; i < Arcs::_NUM_OF_TESTS; ++i) {
      auto tree1{Tree::get_random(2, num_of_points >> 1)};
      auto tree2{tree1->to_arcs()->to_tree()};
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
                             num_of_points, Arcs::_NUM_OF_TESTS);
  }

  std::cout << "\n\nAll Tests Completed!\n\n";
}
