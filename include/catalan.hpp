#pragma once

#include <format>
#include <string>
#include <thread>
#include <unistd.h>

template <class T> class Catalan {
public:
  auto operator<=>(const Catalan &rhs) const = default;
  auto self() { return static_cast<T *>(this); };
  auto self() const { return static_cast<T const *>(this); };

  static void enumerate_avoiding();
  void to_file(std::string file = "");
  void to_svg(std::string outfile);
  void plot() {

    self()->to_file(T::_DEFAULT_PREFIX_FILE);

    if (fork() == 0) {
      std::string python{"./visualizer/bin/python3"};
      std::string plotter{std::format("./visualizer/{}", T::_PLOT_SCRIPT)};
      std::string cmd{
          std::format("{} {} {}", python, plotter, T::_DEFAULT_PREFIX_FILE)};

      using namespace std::chrono_literals;
      std::this_thread::sleep_for(100ms);
      std::system(cmd.c_str());
      std::exit(EXIT_SUCCESS);
    }
  };

private:
};
