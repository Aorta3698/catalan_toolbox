#include "coin.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

std::unique_ptr<CoinStack> CoinStack::of(std::unique_ptr<BaseTree> base_tree) {
  auto tree{Tree::of(std::move(base_tree))};
  return tree->to_coin_stack();
}

void CoinStack::to_file(FileOp mode, std::string file) {
  if (file == "") {
    file = CoinStack::_DEFAULT_PREFIX_FILE;
  }
  auto file_mode =
      mode == FileOp::Truncate ? std::ios_base::trunc : std::ios_base::app;

  std::ofstream out{file, file_mode};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  double scaling{0.5};
  double rad{1 * scaling};
  double up{std::sqrt(3) * rad};

  // meta info
  out << this->base << "\n";
  out << rad << "\n";

  for (const auto &[column, row] : this->coins) {
    out << (column + 1) * rad << "," << row * up << "\n";
  }
}

std::vector<std::array<int, 2>> CoinStack::word_to_coins(const std::string &path) {
  std::vector<std::array<int, 2>> coins;
  int level{};
  for (int i{}; i < int(path.size()); ++i) {
    level += path[i] == '1' ? 1 : -1;
    for (int row{}; row < level; ++row) {
      if (row % 2 != level % 2) {
        coins.push_back({i, row});
      }
    }
  }
  return coins;
}

std::unique_ptr<CoinStack> CoinStack::get_random(int base) {
  return Dyck::get_random(2, base << 1)->to_coin_stack();
}
