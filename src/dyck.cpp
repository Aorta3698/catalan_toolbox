#include "dyck.hpp"
#include "global.hpp"
#include "tree.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

std::unique_ptr<Dyck> Dyck::of(std::unique_ptr<BaseTree> base_tree) {
  auto tree{Tree::of(std::move(base_tree))};
  return tree->to_dyck();
}

std::unique_ptr<Tree> Dyck::to_tree() {
  int k{this->r};
  int level{};
  int id{};
  std::vector<int> last(this->length + 1, -1);
  std::vector<std::unique_ptr<Node>> opens(this->length);
  std::vector<Node *> closes(this->length);
  for (int i{}; i < this->length && k >= 2; ++i) {
    level += path[i] == '1' ? 1 : 1 - k;
    if (level < 0) {
      break;
    }
    if (path[i] == '1') {
      last[level] = i;
      continue;
    } 
    // '0' here
    auto node{std::make_unique<Node>(++id, k)};
    for (int q{}; q < k - 1; ++q) { // assign children
      auto &child = opens[last[level + q + 1] + 1];
      node->children[q] = child ? std::move(child) : std::make_unique<Node>(++id);
    }
    node->children[k - 1] = std::make_unique<Node>(++id);
    int s{last[level + 1]};
    closes[i] = node.get();
    if (s >= 1 && path[s - 1] == '0') { // find parent
      closes[s - 1]->children[k - 1] = std::move(node);
    } else {
      opens[s] = std::move(node);
    }
  }
  if (k < 2 || level != 0) {
    std::cerr << std::format("Error: {} is not a dyck path\n", this->path);
    throw std::invalid_argument("");
  }
  return std::make_unique<Tree>(std::move(opens[0]));
}

std::unique_ptr<CoinStack> Dyck::to_coin_stack() {
  return std::make_unique<CoinStack>(this->path);
}

std::unique_ptr<Dyck> Dyck::get_random(int deg, int length) {
  if (deg <= 1) {
    std::cerr << "Degree for dyck path must be >= 2\n";
    throw std::invalid_argument("");
  }
  int num_of_internal_nodes{length / deg};
  if (length != num_of_internal_nodes * deg) {
    std::cerr << "Warning: Dyck Path length is invalid and truncated.\n";
  }
  auto random_tree{Tree::get_random(deg, num_of_internal_nodes)};
  return random_tree->to_dyck();
}

std::unique_ptr<Dyck> Dyck::next() {
  // TODO
  assert(false);
}

void Dyck::flip_mountain() {
  // TODO
  assert(false);
}

void Dyck::to_file(FileOp mode, std::string file) {
  if (file == "") {
    file = Dyck::_DEFAULT_PREFIX_FILE;
  }
  auto file_mode =
      mode == FileOp::Truncate ? std::ios_base::trunc : std::ios_base::app;

  std::ofstream out{file, file_mode};
  double scale = 0.3;
  double up{1 * scale};
  double down{(this->r - 1) * scale};
  double right{1 * scale};
  double x{};
  double y{};

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

bool Dyck::is_valid(const std::string &path) {
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

void Dyck::test_conversion() {
  std::string dots{};
  int stat_branches[Dyck::_TEST_MAX_BRANCHES + 1];
  std::vector<std::vector<int>> stat_internal_nodes(Dyck::_TEST_MAX_BRANCHES + 1);

  // print title
  std::cout << "====== Dyck: Conversion Test ======\n\n";
  int four_percent{Dyck::_NUM_OF_TESTS / 25};
  std::cout << "100% remaining.";
  std::cout.flush();

  // start testing
  for (int i{Dyck::_NUM_OF_TESTS}; i; --i) {
    int branches{
        std::uniform_int_distribution<>(2, Dyck::_TEST_MAX_BRANCHES)(g_256pp)};
    int nodes{std::uniform_int_distribution<>(1, Dyck::_TEST_MAX_EDGES /
                                                     branches)(g_256pp)};

    auto dyck_path{Dyck::get_random(branches, branches * nodes)};
    auto tree{dyck_path->to_tree()};
    auto copied_path{tree->to_dyck()};

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
  for (int i{2}; i <= Dyck::_TEST_MAX_BRANCHES; ++i) {
    auto &n = stat_internal_nodes[i];
    int sz = n.size();
    int median = (sz & 1) ? n[sz >> 1] : (n[sz >> 1] + n[(sz >> 1) - 1]) >> 1;
    std::cout << std::format("{}\t\t\t{}\t\t\t{}\n", i, median, sz);
  }

  std::cout << std::format("\nAll {} test cases passed!\n", Dyck::_NUM_OF_TESTS);
}
