#include <array>
#include <cassert>
#include <functional>
#include <gmpxx.h>
#include <iostream>
#include <stdexcept>
#include <string>

using mint = mpz_class;

static std::array<mint, 10'001> fact;

static void setup() {
  mint n{1};
  fact[0] = n;
  for (int i{1}; i <= 10'000; ++i) {
    n *= i;
    fact[i] = n;
  }
}

mint combinations(int n, int k) {
  if (n > 10'000) {
    std::cerr << "only support at most 10,000 nodes now\n\n";
    throw std::invalid_argument("");
  }
  return fact[n] / fact[k] / fact[n - k];
}

// std::vector<std::string> gen_tree(int n, int k) {
//   std::vector<std::string> ans{};
//   std::string b(n * k + 1, '0');

//   // y here means number of available leaves, if x >= 1, then we must
//   // reserve 1 spot for internal node, hence the k-1.
//   std::function<void(int, int)> gen = [&](int x, int y) {
//     if (x + y == 0) {
//       ans.push_back(b);
//       return;
//     }
//     if (y) {
//       b[k * (n - x) - y] = '0';
//       gen(x, y - 1);
//     }
//     if (x) {
//       b[k * (n - x) - y] = '1';
//       gen(x - 1, y + k - 1);
//     }
//   };

//   gen(n, 0);
//   return ans;
// }

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

mint rank(std::string bit_string, int k, int num_of_nodes) {
  int count{num_of_nodes - 1};
  mint index{1};

  for (int i{1}; i <= k * (num_of_nodes - 1) && count; ++i) {
    if (bit_string[i] - '0') {
      int pos_n{k * num_of_nodes - i - 1};
      index += combinations(pos_n, count) - combinations(pos_n, count - 1) * (k - 1);
      --count;
    }
  }

  return index;
}

std::string unrank(mint pos, int k, int num_of_nodes) {
  int count{num_of_nodes};
  std::string bit_string(num_of_nodes * k + 1, '0');
  for (int i{}; i < k * num_of_nodes && count; ++i) {
    mint val{combinations(k * num_of_nodes - i - 1, count) -
             combinations(k * num_of_nodes - i - 1, count - 1) * (k - 1)};
    if (pos > val) {
      ++bit_string[i];
      --count;
      pos -= val;
    }
  }
  return bit_string;
}

mint end(int k, int num_of_nodes) {
  return combinations(k * num_of_nodes + 1, num_of_nodes) / (k * num_of_nodes + 1);
}

int main() {
  setup();
  for (int k{2}; k < 6; ++k)
    for (int n{2}; n < 9; ++n) {
      int ok{};
      for (std::string bit_string : gen_tree_1(n, k)) {
        if (mint got{rank(bit_string, k, n)}; got != ++ok) {
          std::cout << std::format("got = {}, need = {}\n", got.get_si(), ok);
          if (ok == 10)
            assert(false);
        }
        if (auto got = unrank(mint(ok), k, n); got != bit_string) {
          std::cout << std::format("got = {}, need = {}\n", got, bit_string);
        }
      }
      if (ok != end(k, n)) {
        assert(false);
      }
    }
};
