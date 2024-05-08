#include "dyck.hpp"
#include "global.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

Node *dyck_path_to_tree(std::string path) {
  if (!is_valid_dyck_path(path)) {
    std::cerr << "Error: This is not a dyck path\n";
    throw std::invalid_argument("");
  }

  // compute number of branches
  int branches = path.size() / std::ranges::count(path, '0');
  // assert(path.size() / branches * branches == path.size());

  // pre-compute some info
  int score{};
  std::unordered_map<int, std::vector<int>> zeros;
  std::vector<int> one(1);
  int cur_one[path.size()];
  std::memset(cur_one, -1, sizeof(cur_one));
  for (int i{}; i < int(path.size()); ++i) {
    if (path[i] == '0') {
      score -= branches - 1;
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
  std::function<Node *(int, int, int)> build = [&](int start, int end,
                                                   int cur_score) {
    if (start > end) {
      return new Node(++id);
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
    auto root{new Node(++id, branches)};
    int cur_child{branches};
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

  return build(0, int(path.size()) - 1, 0);
}

std::string tree_to_dyck_path(const Node *root) {
  std::string encoded_result{};
  std::function<void(const Node *)> encode = [&](const Node *cur_node) {
    int zero{};
    for (const auto next_node : cur_node->children) {
      zero = encoded_result.size();
      encoded_result += "1";
      encode(next_node);
    }
    encoded_result[zero] -= cur_node->is_internal_node();
  };

  encode(root);
  if (!is_valid_dyck_path(encoded_result)) {
    std::cerr << "Error: This tree is not a full k-ary tree\n";
    throw std::invalid_argument("");
  }
  return encoded_result;
}

std::string get_random_dyck_path(int deg, int length) {
  if (deg <= 1) {
    std::cerr << "Degree for dyck path must be >= 2\n";
    throw std::invalid_argument("");
  }
  int num_of_internal_nodes{length / deg};
  if (length != num_of_internal_nodes * deg) {
    std::cerr << "Warning: Dyck Path length is invalid and truncated.\n";
  }
  auto random_tree{get_random_tree(deg, num_of_internal_nodes)};
  std::string dyck_path{tree_to_dyck_path(random_tree)};
  free_tree(random_tree);

  return dyck_path;
}

bool is_valid_dyck_path(std::string path) {
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

void test_conversion_dyck_path() {
  std::string dots{};
  int stat_branches[TEST_MAX_BRANCHES + 1];
  std::vector<std::vector<int>> stat_internal_nodes(TEST_MAX_BRANCHES + 1);

  std::cout << "====== Conversion Test ======\n\n";
  int four_percent{NUM_OF_TESTS / 25};
  std::cout << "100% remaining.";
  std::cout.flush();
  for (int i{NUM_OF_TESTS}; i; --i) {
    int branches{std::uniform_int_distribution<>(2, TEST_MAX_BRANCHES)(g_256ss)};
    int nodes{
        std::uniform_int_distribution<>(1, TEST_MAX_EDGES / branches)(g_256ss)};

    std::string dyck_path{get_random_dyck_path(branches, branches * nodes)};
    auto tree{dyck_path_to_tree(dyck_path)};
    std::string copied_path{tree_to_dyck_path(tree)};

    free_tree(tree);

    ++stat_branches[branches];
    stat_internal_nodes[branches].push_back(nodes);

    if (copied_path != dyck_path) {
      std::cout << "\r";
      std::cout << "dyck path:   " << dyck_path << "\n";
      std::cout << "copied path: " << copied_path << "\n";
      assert(false);
    }

    if (int c = (i - 1) / four_percent; c * four_percent == i - 1) {
      std::cout << "\r";
      dots += ".";
      std::cout << dots + std::to_string(c * 4) + "% remaining.";
      std::cout.flush();
    }
  }
  std::cout << "\n\n";

  for (auto &v : stat_internal_nodes) {
    std::ranges::sort(v);
  }

  std::cout << "Degree\t\tInternal Nodes (Median)\t\tNumber of Tests\n";
  for (int i{2}; i <= TEST_MAX_BRANCHES; ++i) {
    auto &n = stat_internal_nodes[i];
    int sz = n.size();
    int median = (sz & 1) ? n[sz >> 1] : (n[sz >> 1] + n[(sz >> 1) - 1]) >> 1;
    std::cout << i << "\t\t\t" << median << "\t\t\t" << sz << "\n";
  }

  std::cout << "\nAll " << NUM_OF_TESTS << " test cases passed!\n";
}

void test_expected_height() {
  int num_of_samples{10'000};
  int num_of_internal_nodes{100'000};
  int ten{num_of_samples / 10};
  double tolerance{0.02};
  std::cout << "====== Sampling Distribution Test ======\n\n";
  std::cout << "Testing from degree 2 to 5 with 10k random samples and 100k "
               "internal nodes\n\n";
  for (int deg = 2; deg <= 5; ++deg) {
    double sum{};
    for (int c{}; c < num_of_samples; ++c) {
      if (c / ten * ten == c) {
        std::cout << "\r";
        std::cout << "Deg " << deg << " is " << c / ten * 10 << "% completed...";
        std::cout.flush();
      }
      auto tree{get_random_tree(deg, num_of_internal_nodes)};
      sum += height(tree);
      free_tree(tree);
    }
    double res{sum / num_of_samples};
    double expected{asymptote(deg, num_of_internal_nodes)};
    double error{std::abs(res - expected) / expected};
    std::cout << "\r";
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << "Result: " << res << "  |  Expected: " << expected;
    std::cout << " (Allowed Tolerance: 2.00%, got: " << error * 100 << "%)\n";
    if (error > tolerance) {
      std::cerr << "Error: Exceeded the acceptable tolerance.\n";
      assert(false);
    }
  }

  std::cout << "\n\n";
}
