#include "coin.hpp"
#include "util.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

void CoinStack::plot(std::string file) {
  if (file == "") {
    file = CoinStack::_DEFAULT_PREFIX_FILE;
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
  out << this->base << "\n";
  out << rad << "\n";

  // y, x
  std::unordered_map<int, int> prev_map{};
  prev_map[0] = 0;
  for (const auto dir : this->path) {
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

  Util::plot(CoinStack::_PLOT_SCRIPT, file);
}

CoinStack *CoinStack::get_random(int base) {
  return DyckPre::get_random(2, base << 1)->into_coin_stack();
}
