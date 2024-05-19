#pragma once

#include <stdexcept>
#include <string>

#include "dyck_pre.hpp"

class DyckPre;

class CoinStack {
public:
  CoinStack(const std::string path) {
    if (!DyckPre::is_valid(path)) {
      std::cerr << "Error: Coins constructor - given path isn't a dyck path\n";
      throw std::invalid_argument("");
    }
    this->path = path;
    this->base = int(path.size()) >> 1;
  }

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
   * Get a random coin stack
   *
   * @param base:   base length for the coin stack
   *
   * @return A coin stack
   */
  static CoinStack *get_random(int base);

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
