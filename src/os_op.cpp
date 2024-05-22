#include <format>
#include <string>
#include <thread>
#include <unistd.h>

#include "os_op.hpp"

void OsOp::write_to_file(std::string outfile, std::string content, FileOp file_op) {}

void OsOp::to_svg(std::string outfile) {
  to_file(FileOp::Truncate, data_file);

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
  to_file(FileOp::Truncate, data_file);
  _just_plot(this->data_file);
}

void OsOp::_just_plot(std::string data_file) {
  if (fork() == 0) {
    std::string python{"./visualizer/bin/python3"};
    std::string plotter{std::format("./visualizer/{}", this->plot_script)};
    std::string cmd{std::format("{} {} {}", python, plotter, data_file)};

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    std::system(cmd.c_str());
    std::exit(EXIT_SUCCESS);
  }
}
