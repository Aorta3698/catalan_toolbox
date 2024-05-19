#pragma once

#include "tree.hpp"
#include <string>

class CoinStack {
public:
  CoinStack(const DyckPre *const dyck_path) {
    if (2 != dyck_path->get_r()) {
      std::cerr << "Error: Coins constructor - dyck path given must have r = "
                   "2\n";
      throw std::invalid_argument("");
    }

    this->path = dyck_path->get_path();
    this->base = dyck_path->size() >> 1;
  }

  // loved c++20
  auto operator<=>(const CoinStack &rhs) const = default;

  /**
   * Plot the current coin stack.
   *
   * @param file: file to store info needed for graphing it - _DEFAULT_PREFIX_FILE as
   * the default.
   */
  void plot(std::string file = "");

  /**
   * get the next coin stack graph object
   *
   * @return The next coin stack graph object
   *
   */
  CoinStack *next();

private:
  static constexpr std::string _PLOT_SCRIPT{"plot-coins.py"};
  static constexpr std::string _DEFAULT_PREFIX_FILE{".coins"};

  std::string path;
  int base;
};
