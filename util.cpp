#include "util.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numbers>
#include <sstream>

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

int get_catalan(int num) {
  auto cata = std::to_array({1,       1,        2,         5,         14,
                             42,      132,      429,       1430,      4862,
                             16796,   58786,    208012,    742900,    2674440,
                             9694845, 35357670, 129644790, 477638700, 1767263190});
  return cata.at(num);
}

std::array<double, 2> rotate(double x, double y, double deg) {
  double radian{deg * (std::numbers::pi / 180)};

  double nx{x * std::cos(radian) - y * std::sin(radian)};
  double ny{x * std::sin(radian) + y * std::cos(radian)};

  return {nx, ny};
}
