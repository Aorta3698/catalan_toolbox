#include "bracketing.hpp"
#include "poly.hpp"
#include "tree.hpp"

#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

std::unique_ptr<Bracketing> Bracketing::of(std::unique_ptr<BaseTree> base_tree) {
  auto tree{Tree::of(std::move(base_tree))};
  return tree->to_bracketing();
}

std::unique_ptr<Tree> Bracketing::to_tree() {
  if (!Bracketing::is_valid(this->bracketing)) {
    std::cerr << "Error: not a valid bracketing\n";
    throw std::invalid_argument("");
  }
  int cur{};
  int id{};

  std::function<std::unique_ptr<Node>()> build = [&]() {
    auto node{std::make_unique<Node>(id++, 2)};
    for (auto &child : node->children) {
      if (this->bracketing[cur++] == 'x') {
        child = std::make_unique<Node>(id++); // leaf
      } else {
        child = build();
        ++cur; // skip ')'
      }
    }
    return node;
  };

  return std::make_unique<Tree>(build());
}

std::unique_ptr<Poly> Bracketing::to_poly() {
  if (!Bracketing::is_valid(this->bracketing)) {
    std::cerr << "Error: not a valid bracketing\n";
    throw std::invalid_argument("");
  }
  Graph poly;
  std::vector<int> stack;
  int cur{};

  // walk the cut corners, with the root side's pair
  for (const auto c : "(" + this->bracketing + ")") {
    if (c == '(') {
      stack.push_back(cur);
    } else if (c == ')') {
      poly.push_back({stack.back(), cur});
      stack.pop_back();
    } else {
      ++cur;
    }
  }
  return std::make_unique<Poly>(poly);
}

std::unique_ptr<Bracketing> Bracketing::get_random(int num_of_factors) {
  auto tree{Tree::get_random(2, num_of_factors - 1)};
  auto res{tree->to_bracketing()};
  return res;
}

void Bracketing::to_file(FileOp mode, std::string file) {
  if (file == "") {
    file = Bracketing::_DEFAULT_PREFIX_FILE;
  }
  auto file_mode =
      mode == FileOp::Truncate ? std::ios_base::trunc : std::ios_base::app;

  std::ofstream out{file, file_mode};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  out << this->bracketing << "\n";
}

bool Bracketing::is_valid(const std::string &bracketing) {
  std::vector<int> stack{0}; // factors in each open product
  for (const auto c : bracketing) {
    if (c == '(') {
      stack.push_back(0);
    } else if (c == ')' && stack.size() > 1 && stack.back() == 2) { // 2 factors
      stack.pop_back();
      ++stack.back();
    } else if (c == 'x') {
      ++stack.back();
    } else {
      return false;
    }
    if (stack.back() > 2) {
      return false;
    }
  }
  return stack.size() == 1 && stack.back() == 2;
}

void Bracketing::test_conversion() {
  std::cout << "====== Bracketing: Conversion Test ======\n\n";
  std::cout
      << "Starting testing conversion between bracketing and tree with num_of_sides "
         "from 3 to "
      << Bracketing::_TEST_MAX_SIDES << "\n\n";

  for (int num_of_sides{3}; num_of_sides <= Bracketing::_TEST_MAX_SIDES;
       ++num_of_sides) {
    for (int i{}; i < Bracketing::_NUM_OF_TESTS; ++i) {
      auto tree1{Tree::get_random(2, num_of_sides - 2)};
      auto tree2{tree1->to_bracketing()->to_poly()->to_tree()};
      auto tree3{tree1->to_poly()->to_bracketing()->to_tree()};
      std::string id1{tree1->serialize()};
      std::string id2{tree2->serialize()};
      std::string id3{tree3->serialize()};
      if (id1 != id2 || id1 != id3) {
        std::cerr << "Test Failed:\n";
        std::cerr << std::format("sides = {}\n", num_of_sides);
        std::cerr << std::format("id1 = {}\n", id1);
        std::cerr << std::format("id2 = {}\n", id2);
        std::cerr << std::format("id3 = {}\n", id3);
        assert(false);
      }
    }
    std::cout << std::format("Total sides = {:3} done for {} random tests!\n",
                             num_of_sides, Bracketing::_NUM_OF_TESTS);
  }

  std::cout << "\n\nAll Tests Completed!\n\n";
}
