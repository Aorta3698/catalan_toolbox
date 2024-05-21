#include "dyck_pre.hpp"
#include "global.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

std::unique_ptr<Tree> DyckPre::to_tree() {
  // pre-compute some info
  int score{};
  std::unordered_map<int, std::vector<int>> zeros;
  std::vector<int> one(1);
  int cur_one[this->length];
  std::memset(cur_one, -1, sizeof(cur_one));
  for (int i{}; i < this->length; ++i) {
    if (path[i] == '0') {
      score -= this->r - 1;
      zeros[score].push_back(i);
    } else { // path[i] == '1'
      if (++score == int(one.size())) {
        one.emplace_back();
      }
      one[score] = i;
    }
    cur_one[i] = one[score];
  }

  // construct the tree
  int id{};
  std::function<std::unique_ptr<Node>(int, int, int)> build = [&](int start, int end,
                                                                  int cur_score) {
    if (start > end) {
      return std::make_unique<Node>(++id);
    }
    // this assertion must hold
    // maybe a better name for "end" is "cur"
    assert(path[end] == '0');

    // find right-most child split point
    auto &cur_zero = zeros[cur_score];
    int hi{int(cur_zero.size()) - 1};
    int low{};
    while (low < hi) {
      int mid = (hi + low) >> 1;
      if (cur_zero[mid] >= start) {
        hi = mid;
      } else {
        low = mid + 1;
      }
    }

    // assign right-most child
    auto root{std::make_unique<Node>(++id, this->r)};
    int cur_child{this->r};
    root->children[--cur_child] = build(cur_zero[low] + 1, end, cur_score);

    // assign rest of children from the right
    end = cur_zero[low] - 1;
    while (cur_child--) {
      root->children[cur_child] =
          build(cur_one[end] + 1, end, cur_score + cur_child + 1);
      end = cur_one[end] - 1;
    }

    return root;
  };

  return std::make_unique<Tree>(build(0, this->length - 1, 0));
}

std::unique_ptr<CoinStack> DyckPre::to_coin_stack() {
  return std::make_unique<CoinStack>(this->path);
}

std::unique_ptr<DyckPre> DyckPre::get_random(int deg, int length) {
  if (deg <= 1) {
    std::cerr << "Degree for dyck path must be >= 2\n";
    throw std::invalid_argument("");
  }
  int num_of_internal_nodes{length / deg};
  if (length != num_of_internal_nodes * deg) {
    std::cerr << "Warning: Dyck Path length is invalid and truncated.\n";
  }
  auto random_tree{Tree::get_random(deg, num_of_internal_nodes)};
  return random_tree->to_dyck_pre();
}

std::unique_ptr<DyckPre> DyckPre::next() {
  // TODO
  assert(false);
}

void DyckPre::flip_mountain() {
  // TODO
  assert(false);
}

void DyckPre::to_file(std::string file) {
  if (file == "") {
    file = DyckPre::_DEFAULT_PREFIX_FILE;
  }

  double scale = 0.3;
  double up{1 * scale};
  double down{(this->r - 1) * scale};
  double right{1 * scale};
  double x{};
  double y{};

  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  // scaling
  out << scale << "\n";

  // number of points
  out << 1 + this->length << "\n";

  // coordinations
  out << x << "," << y << "\n";
  for (const auto d : this->path) {
    if (d == '1') {
      y += up;
    } else {
      y -= down;
    }
    x += right;
    out << x << "," << y << "\n";
  }

  // edges
  for (int i{}; i < this->length; ++i) {
    out << i << "," << i + 1 << "\n";
  }
}

bool DyckPre::is_valid(const std::string &path) {
  int zeros = std::ranges::count(path, '0');
  int ones = std::ranges::count(path, '1');
  int sz = path.size();
  if (zeros == 0) { // must contain 0
    return false;
  }
  int down{-ones / zeros};
  int score{};
  if (zeros + ones != sz) { // only contains 1 and 0
    return false;
  }
  for (const auto c : path) { // never become < 0
    score += c == '1' ? 1 : down;
    if (score < 0) {
      return false;
    }
  }
  return score == 0;
}

void DyckPre::test_conversion() {
  std::string dots{};
  int stat_branches[DyckPre::_TEST_MAX_BRANCHES + 1];
  std::vector<std::vector<int>> stat_internal_nodes(DyckPre::_TEST_MAX_BRANCHES + 1);

  // print title
  std::cout << "====== DyckPre: Conversion Test ======\n\n";
  int four_percent{DyckPre::_NUM_OF_TESTS / 25};
  std::cout << "100% remaining.";
  std::cout.flush();

  // start testing
  for (int i{DyckPre::_NUM_OF_TESTS}; i; --i) {
    int branches{
        std::uniform_int_distribution<>(2, DyckPre::_TEST_MAX_BRANCHES)(g_256ss)};
    int nodes{std::uniform_int_distribution<>(1, DyckPre::_TEST_MAX_EDGES /
                                                     branches)(g_256ss)};

    auto dyck_path{DyckPre::get_random(branches, branches * nodes)};
    auto tree{dyck_path->to_tree()};
    auto copied_path{tree->to_dyck_pre()};

    ++stat_branches[branches];
    stat_internal_nodes[branches].push_back(nodes);

    if (*copied_path != *dyck_path) {
      std::cout << "\r";
      std::cout << "original: ";
      dyck_path->print();
      std::cout << "copied:   ";
      copied_path->print();
      assert(false);
    }

    if (int c = (i - 1) / four_percent; c * four_percent == i - 1) {
      std::cout << "\r";
      dots += ".";
      std::cout << std::format("{}{}% remaining.", dots, c * 4);
      std::cout.flush();
    }
  }
  std::cout << "\n\n";

  for (auto &v : stat_internal_nodes) {
    std::ranges::sort(v);
  }

  std::cout << "Degree\t\tInternal Nodes (Median)\t\tNumber of Tests\n";
  for (int i{2}; i <= DyckPre::_TEST_MAX_BRANCHES; ++i) {
    auto &n = stat_internal_nodes[i];
    int sz = n.size();
    int median = (sz & 1) ? n[sz >> 1] : (n[sz >> 1] + n[(sz >> 1) - 1]) >> 1;
    std::cout << std::format("{}\t\t\t{}\t\t\t{}\n", i, median, sz);
  }

  std::cout << std::format("\nAll {} test cases passed!\n", DyckPre::_NUM_OF_TESTS);
}
