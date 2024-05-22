#include <format>
#include <string>
#include <thread>
#include <unistd.h>

#include "os_op.hpp"

void OsOp::to_svg(std::string outfile) {
  to_file(data_file);

  if (fork() == 0) {
    std::string python{"./visualizer/bin/python3"};
    std::string plotter{std::format("./visualizer/{}", this->plot_script)};
    std::string cmd{
        std::format("{} {} {} {}", python, plotter, this->data_file, outfile)};

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}

void OsOp::plot() {
  to_file(data_file);

  if (fork() == 0) {
    std::string python{"./visualizer/bin/python3"};
    std::string plotter{std::format("./visualizer/{}", this->plot_script)};
    std::string cmd{std::format("{} {} {}", python, plotter, this->data_file)};

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}
