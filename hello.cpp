#include <cassert>
#include <functional>
#include <iostream>
#include <string>

std::vector<std::string> gen_tree(int n, int k) {
  std::vector<std::string> ans{};
  std::string b(n * k + 1, '0');

  // y here means number of available leaves, if x >= 1, then we must
  // reserve 1 spot for internal node, hence the k-1.
  std::function<void(int, int)> gen = [&](int x, int y) {
    if (x + y == 0) {
      ans.push_back(b);
      return;
    }
    if (y) {
      b[k * (n - x) - y] = '0';
      gen(x, y - 1);
    }
    if (x) {
      b[k * (n - x) - y] = '1';
      gen(x - 1, y + k - 1);
    }
  };

  gen(n, 0);
  return ans;
}

std::vector<std::string> gen_tree_1(int n, int k) {
  std::vector<std::string> ans{};
  std::string b(n * k + 1, '0');
  std::function<void(int, int)> gen = [&](int x, int y) {
    if (y == 0) {
      ans.push_back(b);
      return;
    }
    if (y + (!x) > 1) {
      b[k * (n - x) - y + 1] = '0';
      gen(x, y - 1);
    }
    if (x) {
      b[k * (n - x) - y + 1] = '1';
      gen(x - 1, y + k - 1);
    }
  };

  gen(n, 1);
  return ans;
}

int main() {
  for (int k{2}; k < 6; ++k)
    for (int i{}; i < 10; ++i) {
      auto count{gen_tree_1(i, 4)};
      auto n{gen_tree(i, 4)};
      if (n != count) {
        assert(false);
      }
    }
};
