#include "util.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <numbers>
#include <sstream>
#include <thread>
#include <unistd.h>

template <typename T> void Util::print_vector(const T &t) {
  std::copy(t.cbegin(), t.cend(),
            std::ostream_iterator<typename T::value_type>(std::cerr, ", "));
  std::cerr << "\n";
}

template <typename T> void Util::print_vvector(const T &t) {
  std::for_each(t.cbegin(), t.cend(), Util::print_vector<typename T::value_type>);
}

template <typename T> void Util::print_c_array(const T array[], int size) {
  for (int i{}; i < size; ++i) {
    std::cerr << array[i] << " ";
  }
  std::cerr << "\n";
}

std::vector<std::string> Util::split_string(std::string s, char delimiter) {
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

int Util::get_num(std::string s) {
  size_t processed_count{};
  int num{std::stoi(s, &processed_count)};
  if (processed_count != s.size() || num < 1) {
    throw std::invalid_argument("num processing failure");
  }
  return num;
}

std::string &Util::strip(std::string &str) {
  str.erase(str.find_last_not_of(' ') + 1);
  str.erase(0, str.find_first_not_of(' '));
  return str;
}

int Util::get_catalan(int num) {
  auto cata = std::to_array({1,       1,        2,         5,         14,
                             42,      132,      429,       1430,      4862,
                             16796,   58786,    208012,    742900,    2674440,
                             9694845, 35357670, 129644790, 477638700, 1767263190});
  return cata.at(num);
}

std::array<double, 2> Util::rotate(double x, double y, double deg) {
  double radian{deg * (std::numbers::pi / 180)};

  double nx{x * std::cos(radian) - y * std::sin(radian)};
  double ny{x * std::sin(radian) + y * std::cos(radian)};

  return {nx, ny};
}

void Util::plot(std::string script, std::string file) {
  if (fork() == 0) { // TODO: calling fork inside thread is not good
    std::string python{"./visualizer/bin/python3 "};
    std::string plotter{"./visualizer/" + script + " "};
    std::string cmd{python + plotter + file};
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}

std::vector<Mutze::Pattern> Util::get_avoid_patterns() {
  std::vector<Mutze::Pattern> patterns;
  std::string input{};

  std::cout << "Each pattern is a pair of preorder\n";
  std::cout << "permutation and corresponding edge type list, separated by\n";
  std::cout << "comma; multiple patterns are separated by semicolon\n\n";
  std::cout << "Edge type: 1 -> contiguous, 0 -> non-contiguous\n\n";
  std::cout << "For examples: '2134,000; 2143,111' and '2134,000; 15234,1010'\n\n";

  std::cout << "Only friendly patterns will be accepted. A friendly pattern is one "
               "where:\n";
  std::cout << "1. The largest vertex is neither the root nor a leaf\n";
  std::cout << "2. The edges on the right branch starting at the root, except "
               "possibly the last one, are all non-contiguous\n";
  std::cout << "3. If the edge from k to its parent is contiguous, then the edge to "
               "its left child must be non-contiguous.\n\n";

  while (1) {
    std::cout << "> ";
    std::cout.flush();
    std::cin >> input;
    Mutze::Pattern::read_patterns(input, patterns);
    if (std::all_of(patterns.begin(), patterns.end(),
                    [](auto &p) { return p.is_friendly(); })) {
      break;
    }
    std::cout << "At least one pattern given is not friendly. Try again.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    patterns.clear();
  }

  return patterns;
}
