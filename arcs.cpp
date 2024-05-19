#include "arcs.hpp"
#include "chords.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

Chords *Arcs::to_chords() {
  auto chords{new Chords(this->arcs)};
  return chords;
}

Chords *Arcs::into_chords() {
  auto chords{new Chords(this->arcs)};
  delete this;
  return chords;
}

Tree *Arcs::to_tree() {
  auto chords(this->to_chords());
  return chords->into_tree();
}

Tree *Arcs::into_tree() {
  auto chords{Arcs::to_chords()};
  delete this;
  return chords->into_tree();
}

Arcs *Arcs::get_random(int num_of_points) {
  return Chords::get_random(num_of_points)->into_arcs();
}

void Arcs::plot(std::string file) {
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

  Util::plot(Arcs::_PLOT_SCRIPT, file);
}

Arcs *Arcs::next() {
  // TODO
  assert(false);
}

void Arcs::exchage_arcs() {
  // TODO
  assert(false);
}

bool Arcs::is_valid(const Graph &graph) {
  // TODO
  assert(false);
}

void Arcs::test_conversion() {
  std::cout << "Starting testing conversion between arcs and tree with total points "
               "from 2 to "
            << Arcs::_TEST_MAX_SIDES << " with 2 increment\n\n";

  for (int num_of_points{2}; num_of_points <= Arcs::_TEST_MAX_SIDES;
       num_of_points += 2) {
    for (int i{}; i < Arcs::_NUM_OF_TESTS; ++i) {
      auto tree1{Tree::get_random(2, num_of_points >> 1)};
      auto tree2{tree1->to_arcs()->into_tree()};
      std::string id1{tree1->serialize()};
      std::string id2{tree2->serialize()};
      if (id1 != id2) {
        std::cerr << "Test Failed:\n"
                  << "Total Points = " << num_of_points << "\n"
                  << "id1 = " << id1 << "\n"
                  << "id2 = " << id2 << "\n";
        assert(false);
      }
      tree1->free_memory();
      tree2->free_memory();
      delete tree1;
      delete tree2;
    }
    std::cout << "Total points = " << num_of_points << " done for "
              << Arcs::_NUM_OF_TESTS << " random tests!\n";
  }

  std::cout << "\n\nAll Tests Completed!\n\n";
}
