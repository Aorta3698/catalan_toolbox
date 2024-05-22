#pragma once

#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>

#include "base_tree.hpp"
#include "ktree_iter.hpp"
#include "mutze_tree.hpp"
#include "util.hpp"

template <class T> class Catalan {
public:
  auto operator<=>(const Catalan &rhs) const = default;
  auto self() { return static_cast<T *>(this); };
  auto self() const { return static_cast<T const *>(this); };

  // ------------ child functions -------------
  static std::unique_ptr<T> of(const std::string &);

  std::unique_ptr<T> next_lexi() {
    auto cur{self()};
    std::string bit_str{cur->to_tree()->to_bitstring()};
    auto [k, n] = Lexi::get_k_n(bit_str);
    auto pos{Lexi::rank(bit_str, k, n) + 1};
    auto [next_bit_str, ok] = Lexi::unrank(pos, k, n);
    if (ok == Lexi::NOT_OK) {
      std::cout << "The current Catalan structure is already the last one in "
                   "lexicographical order.\n";
      return cur;
    }
    return T::of_bit(next_bit_str);
  }

  /**
   * Enumerate pattern avoiding Catalan structures.
   */
  static void enumerate_avoiding() {
    auto patterns{Util::get_avoid_patterns()};
    int n{};
    std::string file;
    std::cout << "\nEnter total number of nodes: ";
    std::cin >> n;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the desired filename prefix: ";
    std::getline(std::cin, file);
    std::cin.clear();

    std::cout << "\n\nCurrent setting is 1 second per svg.\n";
    int count{};
    auto mutze_tree = Mutze::Tree(n, patterns);
    auto ok{1};

    while (ok) {
      // steal cout
      std::stringstream buffer;
      std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
      mutze_tree.print();
      std::cout.rdbuf(old);

      // get the corresponding catalan structure
      auto tree{BaseTree::get_from_traversal(buffer.str())};
      auto cat{T::of(std::move(tree))};

      // output the svg
      cat->to_svg(file + std::to_string(count++));
      ok &= mutze_tree.next();
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1s);
      std::cout << "\r";
      std::cout << std::format("{} svg done!", count);
    }
    std::cout << "\n\nAll done!\n\n";
  }
};
