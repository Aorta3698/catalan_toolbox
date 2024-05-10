#include "coin.hpp"
#include "dyck.hpp"
#include "global.hpp"
#include "util.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

void plot_coin_stack(const Dyck dyck_path, std::string file) {
  if (2 != get_r(dyck_path)) {
    std::cerr << "Error: dyck path given must have r = 2\n";
    throw std::invalid_argument("");
  }
  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  double scaling{0.5};
  double rad{1 * scaling};
  double up{std::sqrt(3) * rad};

  // using double is iffy (double type key in unordered_map is kinda cringe)
  int x_steps{};
  int y_steps{};

  // meta info
  out << (int(dyck_path.size()) >> 1) << "\n";
  out << rad << "\n";

  // y, x
  std::unordered_map<int, int> prev_map{};
  prev_map[0] = 0;
  for (const auto dir : dyck_path) {
    if (dir == '1') {
      ++y_steps;
    } else {
      --y_steps;
      for (int cur_x{x_steps}, end{prev_map[y_steps]}; cur_x > end; cur_x -= 2) {
        out << cur_x * rad << "," << y_steps * up << "\n";
      }
    }
    ++x_steps;
    prev_map[y_steps] = x_steps;
  }

  plot(COIN_PLOT_SCRIPT, file);
}

void plot_all_coin_stacks(int n) {
  std::unordered_set<Dyck> seen{};
  int cat{n};
  int total{get_catalan(cat)};
  while (int(seen.size()) < total) {
    Dyck dyck_path = get_random_dyck_path(2, n << 1);
    if (!seen.contains(dyck_path)) {
      plot_coin_stack(dyck_path, ".coins" + std::to_string(int(seen.size())));
      seen.insert(dyck_path);
    }
  }
}

void plot_random_coin_stack(int n, int count) {
  while (count--) {
    Dyck dyck_path{get_random_dyck_path(2, n << 1)};
    plot_coin_stack(dyck_path, ".coins" + std::to_string(count));
  }
}
