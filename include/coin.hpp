#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>

#include "catalan.hpp"
#include "dyck.hpp"

class Dyck;

class CoinStack : public Catalan<CoinStack>, public OsOp {
public:
  CoinStack(const std::string path) : OsOp(_PLOT_SCRIPT, _DEFAULT_PREFIX_FILE) {
    // TODO: check the path
    this->path = path;
    this->base = int(path.size()) >> 1;
    this->coins = CoinStack::word_to_coins(path);
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
   * List the coins of the coin stack of a dyck path, column by column
   *
   * @param path:   the dyck path
   *
   * @return The coins as (column, row) pairs
   */
  static std::vector<std::array<int, 2>> word_to_coins(const std::string &path);

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
  std::vector<std::array<int, 2>> coins;
};
