#include "arcs.hpp"
#include "chords.hpp"
#include "coin.hpp"
#include "dyck_mirrored.hpp"
#include "dyck_pre.hpp"
#include "main.hpp"
#include "poly.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

int main(int argc, const char *argv[]) {
  if (!init()) {
    std::cerr << "init failed\n";
    return -1;
  }

  std::cout << "0 - triangulation\n"
            << "1 - chords\n"
            << "2 - arcs\n"
            << "3 - dyck path pre-order\n"
            << "4 - coins\n"
            << "5 - dyck path pre-order mirrored\n"
            << "6 - trees\n\n";

  std::string prev_dyck_path{};
  std::string prev_cmd{};
  do {
    std::string input{};
    std::cout << "> ";
    std::getline(std::cin, input);
    if (input == "r") {
      input = prev_cmd;
    } else {
      prev_cmd = input;
    }
    std::vector<std::string> tokens{Util::split_string(input, ' ')};

    try {
      std::string cmd = tokens.at(0);
      if (cmd == "test" || cmd == "t") {
        Tree::test_expected_height();
        DyckPre::test_conversion();
        DyckPreMirrored::test_conversion();
        Poly::test_conversion();
        Chords::test_conversion();
        Arcs::test_conversion();
      } else if (cmd == "6r") {
        // SKIP
      } else if (cmd == "6t") {
        // skip
      } else if (cmd == "6m") {
        Tree::enumerate_avoiding();
        //----------------------------------
      } else if (cmd == "5r") {
        // SKIP
      } else if (cmd == "5t") {
        DyckPreMirrored::test_conversion();
      } else if (cmd == "5e") {
        // SKIP
        //----------------------------------
      } else if (cmd == "4r") {
        int base{Util::get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : Util::get_num(tokens.at(3))};
        while (count--) {
          auto coins{CoinStack::get_random(base)};
          coins->plot();
        }
      } else if (cmd == "4t") {
        // SKIP
      } else if (cmd == "4m") {
        CoinStack::enumerate_avoiding();
        //----------------------------------
      } else if (cmd == "3r") {
        int r{Util::get_num(tokens.at(1))};
        int length{Util::get_num(tokens.at(2))};
        int count{tokens.size() == 3 ? 1 : Util::get_num(tokens.at(3))};
        while (count--) {
          auto dyck_pre{DyckPre::get_random(r, length)};
          dyck_pre->plot();
        }
      } else if (cmd == "3t") {
        DyckPre::test_conversion();
      } else if (cmd == "3m") {
        DyckPre::enumerate_avoiding();
      } else if (cmd == "3e") {
        int r{Util::get_num(tokens.at(1))};
        int length{Util::get_num(tokens.at(2))};
        auto dyck_pre{DyckPre::get_random(r, length)};
        dyck_pre->flip_mountain();
        //----------------------------------
      } else if (cmd == "2r") {
        int num_of_points{Util::get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : Util::get_num(tokens.at(2))};
        while (count--) {
          auto arcs{Arcs::get_random(num_of_points)};
          arcs->plot();
        }
      } else if (cmd == "2m") {
        Arcs::enumerate_avoiding();
      } else if (cmd == "2t") {
        Arcs::test_conversion();
      } else if (cmd == "2e") {
        int num_of_points{Util::get_num(tokens.at(1))};
        auto arcs{Arcs::get_random(num_of_points)};
        arcs->exchage_arcs();
        //----------------------------------
      } else if (cmd == "1r") {
        int num_of_points{Util::get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : Util::get_num(tokens.at(2))};
        while (count--) {
          auto chords{Chords::get_random(num_of_points)};
          chords->plot();
        }
      } else if (cmd == "1t") {
        Chords::test_conversion();
      } else if (cmd == "1m") {
        Chords::enumerate_avoiding();
      } else if (cmd == "1e") {
        int num_of_points{Util::get_num(tokens.at(1))};
        auto chords{Chords::get_random(num_of_points)};
        chords->exchage_chords();
        //----------------------------------
      } else if (cmd == "0r") {
        int num_of_sides{Util::get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : Util::get_num(tokens.at(2))};
        while (count--) {
          auto poly{Poly::get_random(num_of_sides)};
          poly->plot();
        }
      } else if (cmd == "0t") {
        Poly::test_conversion();
      } else if (cmd == "0m") {
        Poly::enumerate_avoiding();
      } else if (cmd == "0f") {
        int num_of_sides{Util::get_num(tokens.at(1))};
        auto poly{Poly::get_random(num_of_sides)};
        poly->flip_and_plot();
        //---------------------------------
      } else if (cmd == "quit" || cmd == "q") {
        break;
      } else if (cmd == "gen" || cmd == "g") {
        int r{Util::get_num(tokens.at(1))};
        int len{Util::get_num(tokens.at(2))};
        auto dyck_pre{DyckPre::get_random(r, len)};
        dyck_pre->print();
        prev_dyck_path = dyck_pre->get_path();
      } else if (cmd == "plot" || cmd == "p") {
        std::string dyck_path{tokens.size() == 2 ? tokens.at(1) : prev_dyck_path};
        auto path{std::make_unique<DyckPre>(dyck_path)};
        auto tree{path->to_tree()};
        tree->plot();
      } else if (cmd == "file" || cmd == "f") {
        std::string file{tokens.at(1)};
        auto tree{Tree::get_from_file(file)};
        auto dyck_pre{tree->to_dyck_pre()};
        dyck_pre->print();
        prev_dyck_path = dyck_pre->get_path();
      } else if (cmd == "save" || cmd == "s") {
        std::string dyck_path{tokens.at(1)};
        std::string file{tokens.at(2)};
        auto tree{(new DyckPre(dyck_path))->to_tree()};
        tree->to_file(OsOp::FileOp::Truncate, file);
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

void print_usage() {
  int cmd_spc{30};
  std::vector<std::pair<std::string, std::string>> cmds{
      {"gen <deg> <len>", "(g) Generate a random dyck path"},
      {"plot [dyck path]", "(p) Plot a full k-ary tree from (given/prev) dyck path"},
      {"file <file>", "(e) Read a tree file (1 edge on each line) into dyck path"},
      {"all <num>", "(a) Plot all binary trees with internal nodes <= 4"},
      {"save <dyck path> <file>", "(s) Save the dyck path as a tree in file"},
      {"test", "(t) Run all test cases"},
      {"help", "(h) Print this message"},
      {"quit", "(q) Quit the program"}};

  std::cout << "commands:\n";
  for (auto &[cmd, desc] : cmds) {
    std::cout << "    ";
    std::cout << cmd << std::string(cmd_spc - int(cmd.size()), ' ') << desc << "\n";
  }
}

bool init() {
  Util::setup_comb();
  return true;
}
