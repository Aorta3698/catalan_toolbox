#pragma once

#include <format>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "mutze_tree.hpp"
#include "util.hpp"

template <class T> class Catalan {
public:
  auto operator<=>(const Catalan &rhs) const = default;
  auto self() { return static_cast<T *>(this); };
  auto self() const { return static_cast<T const *>(this); };
  void to_file(std::string file = "");
  static void of(const Mutze::Tree);

  /**
   * Enumerate pattern avoiding Catalan structures.
   */
  static void enumerate_avoiding() {
    auto patterns{Util::get_avoid_patterns()};
    int n{};
    std::string file;
    std::cout << "\nEnter total number of nodes: ";
    std::cin >> n;
    std::cout << "Enter the desired filename prefix: ";
    std::getline(std::cin, file);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int count{};
    auto mutze_tree = Mutze::Tree(n, patterns);
    auto ok{1};
    while (ok) {
      auto cat{T::of(mutze_tree)};
      cat->to_svg(file + std::to_string(count++));
      ok &= mutze_tree.next();
    }
  }

  void to_svg(std::string outfile) {
    if (fork() == 0) {
      std::string python{"./visualizer/bin/python3"};
      std::string plotter{std::format("./visualizer/{}", T::_PLOT_SCRIPT)};
      std::string cmd{std::format("{} {} {} {}", python, plotter,
                                  T::_DEFAULT_PREFIX_FILE, outfile)};

      using namespace std::chrono_literals;
      std::this_thread::sleep_for(100ms);
      std::system(cmd.c_str());
      std::exit(EXIT_SUCCESS);
    }
  }

  void plot() {
    self()->to_file(T::_DEFAULT_PREFIX_FILE);

    if (fork() == 0) {
      std::string python{"./visualizer/bin/python3"};
      std::string plotter{std::format("./visualizer/{}", T::_PLOT_SCRIPT)};
      std::string cmd{
          std::format("{} {} {}", python, plotter, T::_DEFAULT_PREFIX_FILE)};

      using namespace std::chrono_literals;
      std::this_thread::sleep_for(100ms);
      std::system(cmd.c_str());
      std::exit(EXIT_SUCCESS);
    }
  };

private:
};
