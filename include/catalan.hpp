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

template <class T> class Catalan {
public:
  auto operator<=>(const Catalan &rhs) const = default;
  auto self() { return static_cast<T *>(this); };
  auto self() const { return static_cast<T const *>(this); };

  // ------------ child functions -------------
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
    constexpr bool has_toString = requires { T::of; };
    if constexpr (!has_toString) {
      std::cerr << "Aborting: where is static of() function?\n";
      std::exit(-1);
    }

    // take input
    auto patterns{Catalan<T>::get_avoid_patterns()};
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

private:
  /**
   * Set the avoid patterns for Mutze pattern avoiding trees
   *
   * It will only accept friendly patterns.
   *
   * each pattern is a pair of preorder
   * permutation and corresponding edge type list, separated by
   * comma; multiple patterns are separated by semicolon
   *
   * For examples: '2134,000; 2143,111' and '2134,000; 15234,1010'
   */
  static std::vector<Mutze::Pattern> get_avoid_patterns() {
    std::vector<Mutze::Pattern> patterns;
    std::string input{};

    std::cout << "Each pattern is a pair of preorder\n";
    std::cout << "permutation and corresponding edge type list, separated by\n";
    std::cout << "comma; multiple patterns are separated by semicolon\n\n";
    std::cout << "Edge type: 1 -> contiguous, 0 -> non-contiguous\n\n";
    std::cout << "For examples: '2134,000; 2143,111' and '2134,000; 15234,1010'\n\n";

    std::cout
        << "Only friendly patterns will be accepted. A friendly pattern is one "
           "where:\n";
    std::cout << "1. The largest vertex is neither the root nor a leaf\n";
    std::cout << "2. The edges on the right branch starting at the root, except "
                 "possibly the last one, are all non-contiguous\n";
    std::cout
        << "3. If the edge from k to its parent is contiguous, then the edge to "
           "its left child must be non-contiguous.\n\n";

    while (1) {
      std::cout << "> ";
      std::cout.flush();
      std::getline(std::cin, input);
      if (!Mutze::Pattern::read_patterns(input, patterns)) {
        continue;
      }
      if (std::all_of(patterns.begin(), patterns.end(),
                      [](auto &p) { return p.is_friendly(); })) {
        break;
      }
      std::cout << "At least one pattern given is not friendly. Try again.\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      patterns.clear();
    }

    return patterns;
  }
};
