#pragma once

#include <memory>
#include <string>

#include "catalan.hpp"
#include "dyck_pre.hpp"

class DyckPre;

class CoinStack : public Catalan<CoinStack> {
public:
  CoinStack(const std::string path) {
    // TODO: check the path
    this->path = path;
    this->base = int(path.size()) >> 1;
  }

  auto operator<=>(const CoinStack &rhs) const = default;

  /**
   * Constructor a catalan structure from the current mutze tree.
   *
   * @param mtree:  Mutze tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<CoinStack> of(const std::string &mtree);

  /**
   * Get a random coin stack
   *
   * @param base:   base length for the coin stack
   *
   * @return A coin stack
   */
  static std::unique_ptr<CoinStack> get_random(int base);

  /**
   * Store the current coin stack as file
   *
   * @param file: file to store it  - _DEFAULT_PREFIX_FILE as the default.
   */
  void to_file(std::string file = "");

  /**
   * get the next coin stack graph object
   *
   * @return The next coin stack graph object
   *
   */
  std::unique_ptr<CoinStack> next();

  static constexpr std::string _PLOT_SCRIPT{"plot-coins.py"};
  static constexpr std::string _DEFAULT_PREFIX_FILE{".coins"};

private:
  std::string path;
  int base;
};
