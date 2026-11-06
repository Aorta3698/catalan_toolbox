#include "arcs.hpp"
#include "chords.hpp"
#include "coin.hpp"
#include "dyck_pre.hpp"
#include "main.hpp"
#include "poly.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main(int argc, const char *argv[]) {
  if (!init()) {
    std::cerr << "init failed\n";
    return -1;
  }

  print_usage();

  do {
    std::string input{};
    std::cout << "> ";
    if (!std::getline(std::cin, input)) {
      std::cout << "\n";
      break;
    }
    if (Util::strip(input).empty()) {
      continue;
    }
    std::vector<std::string> tokens{Util::split_string(input, ' ')};

    try {
      std::string cmd = form_name(tokens.at(0));
      std::string form{tokens.size() > 1 ? form_name(tokens[1]) : ""};
      bool avoid{cmd == "avoid"};
      if (cmd == "test" && form.empty()) {
        Tree::test_expected_height();
        DyckPre::test_conversion();
        Poly::test_conversion();
        Chords::test_conversion();
        Arcs::test_conversion();
      } else if (cmd == "plot" && form == "tree") {
        int k{Util::get_num(tokens.at(2))};
        int num_of_nodes{Util::get_num(tokens.at(3))};
        int count{tokens.size() == 4 ? 1 : Util::get_num(tokens.at(4))};
        while (count--) {
          auto tree{Tree::get_random(k, num_of_nodes)};
          tree->plot();
        }
      } else if (cmd == "test" && form == "tree") {
        // skip
        //----------------------------------
      } else if (cmd == "plot" && form == "coins") {
        int base{Util::get_num(tokens.at(2))};
        int count{tokens.size() == 3 ? 1 : Util::get_num(tokens.at(3))};
        while (count--) {
          auto coins{CoinStack::get_random(base)};
          coins->plot();
        }
      } else if (cmd == "test" && form == "coins") {
        // SKIP
        //----------------------------------
      } else if (cmd == "plot" && form == "dyck") {
        int r{Util::get_num(tokens.at(2))};
        int length{Util::get_num(tokens.at(3))};
        int count{tokens.size() == 4 ? 1 : Util::get_num(tokens.at(4))};
        while (count--) {
          auto dyck_pre{DyckPre::get_random(r, length)};
          dyck_pre->plot();
        }
      } else if (cmd == "test" && form == "dyck") {
        DyckPre::test_conversion();
        //----------------------------------
      } else if (cmd == "plot" && form == "arcs") {
        int num_of_arcs{Util::get_num(tokens.at(2))};
        int count{tokens.size() == 3 ? 1 : Util::get_num(tokens.at(3))};
        while (count--) {
          auto arcs{Arcs::get_random(2 * num_of_arcs)};
          arcs->plot();
        }
      } else if (cmd == "test" && form == "arcs") {
        Arcs::test_conversion();
        //----------------------------------
      } else if (cmd == "plot" && form == "chords") {
        int num_of_chords{Util::get_num(tokens.at(2))};
        int count{tokens.size() == 3 ? 1 : Util::get_num(tokens.at(3))};
        while (count--) {
          auto chords{Chords::get_random(2 * num_of_chords)};
          chords->plot();
        }
      } else if (cmd == "test" && form == "chords") {
        Chords::test_conversion();
        //----------------------------------
      } else if (cmd == "plot" && form == "triangulation") {
        int num_of_sides{Util::get_num(tokens.at(2))};
        int count{tokens.size() == 3 ? 1 : Util::get_num(tokens.at(3))};
        while (count--) {
          auto poly{Poly::get_random(num_of_sides)};
          poly->plot();
        }
      } else if (cmd == "test" && form == "triangulation") {
        Poly::test_conversion();
      } else if (cmd == "flip") {
        int num_of_sides{Util::get_num(tokens.at(1))};
        auto poly{Poly::get_random(num_of_sides)};
        poly->flip_and_plot();
        //---------------------------------
      } else if ((cmd == "enumerate" || avoid) && form == "tree" && tokens.at(2) == "2") {
        Tree::enumerate_avoiding(Util::get_num(tokens.at(3)), avoid);
      } else if ((cmd == "enumerate" || avoid) && form == "dyck" && tokens.at(2) == "2") {
        int length{Util::get_num(tokens.at(3))};
        if (length % 2) {
          throw std::invalid_argument("");
        }
        DyckPre::enumerate_avoiding(length / 2, avoid);
      } else if ((cmd == "enumerate" || avoid) && form == "coins") {
        CoinStack::enumerate_avoiding(Util::get_num(tokens.at(2)), avoid);
      } else if ((cmd == "enumerate" || avoid) && form == "arcs") {
        Arcs::enumerate_avoiding(Util::get_num(tokens.at(2)), avoid);
      } else if ((cmd == "enumerate" || avoid) && form == "chords") {
        Chords::enumerate_avoiding(Util::get_num(tokens.at(2)), avoid);
      } else if ((cmd == "enumerate" || avoid) && form == "triangulation") {
        int num_of_sides{Util::get_num(tokens.at(2))};
        if (num_of_sides < 3) {
          throw std::invalid_argument("");
        }
        Poly::enumerate_avoiding(num_of_sides - 2, avoid);
      } else if (cmd == "quit") {
        break;
      } else {
        print_hint(tokens);
      }
    } catch (std::exception &e) {
      print_hint(tokens);
    }
  } while (1);

  // TODO
  // core dump when 240 branches + 3000 edges ish
  // test_conversion();
};

void print_usage() {
  int cmd_spc{28};
  std::vector<std::pair<std::string, std::string>> cmds{
      {"plot <form> <size> [count]", "draw random objects of one form"},
      {"enu[merate] <form> <size>", "save every object of one size as an svg"},
      {"flip <vertices>", "flip the diagonals with its dual tree beside it"},
      {"avoid <form> <size>", "list the objects that avoid given patterns"},
      {"test [form]", "run the tests (every form if none is given)"},
      {"help", "show help menu"},
      {"q[uit]", "quit"}};
  int form_spc{22};
  std::vector<std::pair<std::string, std::string>> forms{
      {"FORM", "SIZE"},
      {"tri[angulation]", "vertices"},
      {"chords, arcs", "chords or arcs"},
      {"dyck", "k, then length"},
      {"coins", "coins in the bottom row"},
      {"tree", "k, then internal nodes"}};

  std::cout << "Catalan Toolbox\n\n";
  for (auto &[cmd, desc] : cmds) {
    std::cout << "  ";
    std::cout << cmd << std::string(cmd_spc - int(cmd.size()), ' ') << desc << "\n";
  }
  std::cout << "\n";
  for (auto &[form, size] : forms) {
    std::cout << "  ";
    std::cout << form << std::string(form_spc - int(form.size()), ' ') << size << "\n";
  }
  std::cout << "\n";
}

bool init() {
  // Util::setup_comb();
  return true;
}

void print_hint(const std::vector<std::string> &tokens) {
  std::string cmd{tokens.empty() ? "" : form_name(tokens[0])};
  std::string form{tokens.size() > 1 ? form_name(tokens[1]) : ""};
  if (cmd == "flip") {
    std::cout << "flip needs the number of vertices, e.g. flip 8\n";
  } else if (cmd == "plot" && form == "triangulation") {
    std::cout << "plot triangulation needs the number of vertices, e.g. plot tri 8\n";
  } else if (cmd == "plot" && form == "chords") {
    std::cout << "plot chords needs the number of chords, e.g. plot chords 4\n";
  } else if (cmd == "plot" && form == "arcs") {
    std::cout << "plot arcs needs the number of arcs, e.g. plot arcs 4\n";
  } else if (cmd == "plot" && form == "dyck") {
    std::cout << "plot dyck needs k, then the length, e.g. plot dyck 2 10\n";
  } else if (cmd == "plot" && form == "coins") {
    std::cout << "plot coins needs the coins in the bottom row, e.g. plot coins 5\n";
  } else if (cmd == "plot" && form == "tree") {
    std::cout << "plot tree needs k, then the internal nodes, e.g. plot tree 2 5\n";
  } else if (cmd == "plot") {
    std::cout << "plot needs a form and its size, e.g. plot tri 8\n";
  } else if (cmd == "enumerate") {
    std::cout << "enumerate needs a form and its size as in plot (k = 2 only atm), e.g. enu tri 6\n";
  } else if (cmd == "avoid") {
    std::cout << "avoid needs a form and its size as in plot (k = 2 only atm), e.g. avoid tri 6\n";
  } else if (cmd == "test") {
    std::cout << "test takes a form, e.g. test dyck, or none to run every test\n";
  } else {
    print_usage();
  }
}

std::string form_name(const std::string &word) {
  if (word == "tri") {
    return "triangulation";
  }
  if (word == "enu") {
    return "enumerate";
  }
  if (word == "q") {
    return "quit";
  }
  return word;
}
