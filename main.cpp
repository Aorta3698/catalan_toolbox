#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numbers>
#include <random>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "main.hpp"
#include "xoshiro256.hpp"

// seed generator once
std::random_device rd;
xoshiro256ss g_256ss(rd());

void plot_all_trees(int);

int main(int argc, const char *argv[]) {
  std::string prev_dyck_path{};
  do {
    std::string input{};
    std::cout << "> ";
    std::getline(std::cin, input);
    std::vector<std::string> tokens{split_string(input, ' ')};
    try {
      std::string cmd = tokens.at(0);
      if (cmd == "test" || cmd == "t") {
        test_expected_height();
        test_functionality();
      } else if (cmd == "quit" || cmd == "q") {
        break;
      } else if (cmd == "gen" || cmd == "g") {
        int deg{get_num(tokens.at(1))};
        int len{get_num(tokens.at(2))};
        std::string dyck_path{get_random_dyck_path(deg, len)};
        std::cout << dyck_path << "\n";
        prev_dyck_path = dyck_path;
      } else if (cmd == "plot" || cmd == "p") {
        std::string dyck_path{tokens.size() == 2 ? tokens.at(1) : prev_dyck_path};
        auto tree{dyck_path_to_tree(dyck_path)};
        store_tree_into_file(tree, TREE_OUTPUT_FILENAME);
        free_tree(tree);
        plot_tree(TREE_OUTPUT_FILENAME);
      } else if (cmd == "all" || cmd == "a") {
        int num_of_internal_nodes = get_num(tokens.at(1));
        plot_all_trees(num_of_internal_nodes);
      } else if (cmd == "decode" || cmd == "d") {
        std::string dyck_path{tokens.at(1)};
        auto tree{dyck_path_to_tree(dyck_path)};
        store_tree_into_file(tree, TREE_OUTPUT_FILENAME);
        free_tree(tree);
        plot_tree(TREE_OUTPUT_FILENAME);
      } else if (cmd == "encode" || cmd == "e") {
        std::string file{tokens.at(1)};
        auto tree{get_tree_from_file(file)};
        std::string dyck_path{tree_to_dyck_path(tree)};
        std::cout << dyck_path << "\n";
        free_tree(tree);
        prev_dyck_path = dyck_path;
      } else if (cmd == "save" || cmd == "s") {
        std::string dyck_path{tokens.at(1)};
        std::string file{tokens.at(2)};
        auto tree{dyck_path_to_tree(dyck_path)};
        store_tree_into_file(tree, file);
      } else {
        print_usage();
      }
    } catch (std::exception &e) {
      print_usage();
    }
  } while (1);

  // TODO
  // core dump when 240 branches + 3000 edges ish
  // test_conversion();
};

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

Node *get_random_tree(int branches, int num_nodes) {
  // randomly select internal nodes
  int upper{branches * num_nodes + 1};
  bool seq[upper];
  std::memset(seq, 0, sizeof(seq));
  std::vector<int> indexes(upper);
  std::iota(indexes.begin(), indexes.end(), 0);
  std::shuffle(indexes.begin(), indexes.end(), g_256ss);
  for (int i{}; i < num_nodes; ++i) {
    seq[indexes[i]] = 1;
  }

  // find root
  int score{1};
  int min{2};
  int cur{};
  for (int i{}; i < upper; ++i) {
    score += seq[i] ? branches - 1 : -1;
    if (score < min) {
      min = score;
      cur = i;
    }
  }

  // build tree
  std::function<Node *()> build = [&]() {
    if (++cur == upper) {
      cur = 0;
    }
    auto node{new Node(cur, seq[cur] ? branches : 0)};
    for (auto &child : node->children) {
      child = build();
    }
    return node;
  };

  return build();
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

void store_tree_into_file(const Node *root, std::string file) {
  assert(root);
  std::ofstream out{file};
  if (!out) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  int e{};
  std::function<void(const Node *)> print_edges = [&](const Node *cur_node) {
    for (const auto child : cur_node->children) {
      out << cur_node->id << ',' << child->id << "\n";
      print_edges(child);
      ++e;
    }
  };

  print_edges(root);
  // std::cerr << "edges: " << e << "\n";
  // deconstructor for _out_ is auto called
}

Node *get_tree_from_file(std::string file) {
  std::ifstream in{file};
  if (!in) {
    std::cerr << file << " cannot be opened.\n";
    throw std::invalid_argument("");
  }

  // build graph adj list
  std::unordered_map<int, std::vector<int>> adj{};
  std::unordered_map<int, int> deg{};
  std::string line{};
  while (getline(in, line)) {
    line = strip(line);
    auto tokens{split_string(line, ',')};
    try {
      int v{get_num(tokens[0])};
      int u{get_num(tokens[1])};
      ++deg[v];
      ++deg[u];
      adj[v].push_back(u);
      adj[u].push_back(v);
    } catch (std::exception &e) {
      std::cerr << "Error: Data in file is badly formatted.\n";
      throw std::invalid_argument("");
    }
  }

  // find root - assuming data is sound
  int root{};
  int min = 1e9;
  for (const auto [id, count] : deg) {
    if (count < min) {
      min = count;
      root = min;
    } else if (count < root) {
      root = min;
    }
  }

  // build tree
  int branches{deg[root]};
  std::function<Node *(int, int)> build = [&](int cur_node, int parent) {
    Node *node{new Node(cur_node)};
    for (int child : adj[cur_node]) {
      if (child != parent) {
        node->children.push_back(build(child, cur_node));
      }
    }
    if (int count = node->children.size(); count && count != branches) {
      std::cerr << "Error: This is not a tree formed by a Dyck Path\n";
      throw std::invalid_argument("");
    }
    return node;
  };

  return build(root, -1);
}

void plot_tree(std::string file) {
  if (fork() == 0) { // TODO: calling fork inside thread is not good
    std::string python{"./visualizer/bin/python3 "};
    std::string script{"./visualizer/plot-tree.py "};
    std::string cmd{python + script + file};
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}

void plot_all_trees(int num_of_internal_nodes) {
  std::unordered_set<std::string> seen{};
  while (seen.size() < 14) {
    std::string path{get_random_dyck_path(2, 2 * num_of_internal_nodes)};
    if (!seen.contains(path)) {
      seen.insert(path);
      auto tree{dyck_path_to_tree(path)};
      std::string file{".t" + std::to_string(seen.size())};
      store_tree_into_file(tree, file);
      free_tree(tree);
      plot_tree(file);
    }
  }
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

void free_tree(Node *cur_node) {
  for (auto child : cur_node->children) {
    free_tree(child);
  }
  delete cur_node;
}

template <typename T> void print_vector(const T &t) {
  std::copy(t.cbegin(), t.cend(),
            std::ostream_iterator<typename T::value_type>(std::cerr, ", "));
  std::cerr << "\n";
}

template <typename T> void print_c_array(const T array[], int size) {
  for (int i{}; i < size; ++i) {
    std::cerr << array[i] << " ";
  }
  std::cerr << "\n";
}

std::vector<std::string> split_string(std::string s, char delimiter) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  std::stringstream ss{s};
  std::vector<std::string> ans;
  std::string cur_str{};
  while (std::getline(ss, cur_str, delimiter)) {
    ans.push_back(cur_str);
  }
  return ans;
}

int get_num(std::string s) {
  size_t processed_count{};
  int num{std::stoi(s, &processed_count)};
  if (processed_count != s.size() || num < 1) {
    throw std::invalid_argument("num processing failure");
  }
  return num;
}

std::string &strip(std::string &str) {
  str.erase(str.find_last_not_of(' ') + 1);
  str.erase(0, str.find_first_not_of(' '));
  return str;
}

void test_functionality() {
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

void print_usage() {
  int cmd_spc{30};
  std::vector<std::pair<std::string, std::string>> cmds{
      {"gen <deg> <len>", "(g) Generate a random dyck path"},
      {"plot [dyck path]", "(p) Plot a full k-ary tree from (given/prev) dyck path"},
      {"encode <file>", "(e) Encode a tree (1 edge on each line) into dyck path"},
      {"decode <dyck path>", "(d) Decode a dyck path into tree and then plot it"},
      {"save <dyck path> <file>", "(s) Save the dyck path as a tree in file"},
      {"test", "(t) Run randomized test cases"},
      {"help", "(h) Print this message"},
      {"quit", "(q) Quit the program"}};

  std::cout << "commands:\n";
  for (auto &[cmd, desc] : cmds) {
    std::cout << "    ";
    std::cout << cmd << std::string(cmd_spc - int(cmd.size()), ' ') << desc << "\n";
  }
}

int height(const Node *root) {
  std::function<int(const Node *)> solve = [&](const Node *cur_node) {
    int depth{};
    for (const auto &child : cur_node->children) {
      depth = std::max(depth, solve(child));
    }
    return depth + 1;
  };

  // the book assumes root by itself = 1 height
  return solve(root);
}

double asymptote(int k, int num_of_internal_nodes) {
  return std::pow(std::numbers::pi * 2.0 * num_of_internal_nodes * k / (k - 1), 0.5);
}
