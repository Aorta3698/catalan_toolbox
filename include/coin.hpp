#pragma once

#include <memory>
#include <string>
#include <unistd.h>

#include "catalan.hpp"
#include "dyck_pre.hpp"

class DyckPre;

class CoinStack : public Catalan<CoinStack>, public OsOp {
public:
  CoinStack(const std::string path) : OsOp(_PLOT_SCRIPT, _DEFAULT_PREFIX_FILE) {
    // TODO: check the path
    this->path = path;
    this->base = int(path.size()) >> 1;
  }

  auto operator<=>(const CoinStack &rhs) const = default;

  /**
   * Constructor a catalan structure from the current base tree
   *
   * @param tree:  current base tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<CoinStack> of(std::unique_ptr<BaseTree> tree);

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
  void to_file(FileOp mode = FileOp::Truncate, std::string file = "");

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
