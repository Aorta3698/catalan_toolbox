#include "catalan.hpp"
#include "mutze_tree.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <iostream>
#include <thread>

template <typename T> void Catalan<T>::enumerate_avoiding() {
  auto patterns{Util::get_avoid_patterns()};
  int n{};
  std::string file;
  std::cout << "\nEnter total number of nodes: ";
  std::cin >> n;
  std::cout << "Enter the desired filename prefix: ";
  std::getline(std::cin, file);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  int count{};
  auto mutze_tree = Mutze::Tree(n, patterns);
  auto ok{1};
  while (ok) {
    auto tree{Tree::get_from_Mutze(mutze_tree)};
    auto cat{T::of(tree)};
    cat->to_svg(file + std::to_string(count++));
    ok &= mutze_tree.next();
  }
}

template <typename T> void Catalan<T>::to_svg(std::string outfile) {
  if (fork() == 0) {
    std::string python{"./visualizer/bin/python3"};
    std::string plotter{"./visualizer/" + T::_PLOT_SCRIPT};
    std::string cmd{std::format("{} {} {} {}", python, plotter,
                                T::_DEFAULT_PREFIX_FILE, outfile)};

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}

// template <typename T> void Catalan<T>::plot() {
// this->to_file(T::_DEFAULT_PREFIX_FILE);

// if (fork() == 0) {
//   std::string python{"./visualizer/bin/python3"};
//   std::string plotter{"./visualizer/" + T::_PLOT_SCRIPT};
//   std::string cmd{
//       std::format("{} {} {}", python, plotter, T::_DEFAULT_PREFIX_FILE)};

//   using namespace std::chrono_literals;
//   std::this_thread::sleep_for(100ms);
//   std::system(cmd.c_str());
//   std::exit(EXIT_SUCCESS);
// }
// }
