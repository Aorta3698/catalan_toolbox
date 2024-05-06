#include "dyck.hpp"
#include "main.hpp"
#include "util.hpp"

#include <exception>
#include <iostream>
#include <ranges>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

int main(int argc, const char *argv[]) {
  std::string prev_dyck_path{};
  do {
    std::string input{};
    std::cout << "> ";
    std::getline(std::cin, input);
    std::vector<std::string> tokens{split_string(input, ' ')};
    try {
      std::string cmd = tokens.at(0);
      if (cmd == "test" || cmd == "t") {
        test_expected_height();
        test_functionality();
      } else if (cmd == "quit" || cmd == "q") {
        break;
      } else if (cmd == "gen" || cmd == "g") {
        int deg{get_num(tokens.at(1))};
        int len{get_num(tokens.at(2))};
        std::string dyck_path{get_random_dyck_path(deg, len)};
        std::cout << dyck_path << "\n";
        prev_dyck_path = dyck_path;
      } else if (cmd == "plot" || cmd == "p") {
        std::string dyck_path{tokens.size() == 2 ? tokens.at(1) : prev_dyck_path};
        auto tree{dyck_path_to_tree(dyck_path)};
        store_tree_into_file(tree, TREE_OUTPUT_FILENAME);
        free_tree(tree);
        plot_tree(TREE_OUTPUT_FILENAME);
      } else if (cmd == "all" || cmd == "a") {
        int num_of_internal_nodes = get_num(tokens.at(1));
        plot_all_trees(num_of_internal_nodes);
      } else if (cmd == "decode" || cmd == "d") {
        std::string dyck_path{tokens.at(1)};
        auto tree{dyck_path_to_tree(dyck_path)};
        store_tree_into_file(tree, TREE_OUTPUT_FILENAME);
        free_tree(tree);
        plot_tree(TREE_OUTPUT_FILENAME);
      } else if (cmd == "encode" || cmd == "e") {
        std::string file{tokens.at(1)};
        auto tree{get_tree_from_file(file)};
        std::string dyck_path{tree_to_dyck_path(tree)};
        std::cout << dyck_path << "\n";
        free_tree(tree);
        prev_dyck_path = dyck_path;
      } else if (cmd == "save" || cmd == "s") {
        std::string dyck_path{tokens.at(1)};
        std::string file{tokens.at(2)};
        auto tree{dyck_path_to_tree(dyck_path)};
        store_tree_into_file(tree, file);
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
      {"encode <file>", "(e) Encode a tree (1 edge on each line) into dyck path"},
      {"decode <dyck path>", "(d) Decode a dyck path into tree and then plot it"},
      {"all <num>", "(a) Plot all binary trees with internal nodes <= 4"},
      {"save <dyck path> <file>", "(s) Save the dyck path as a tree in file"},
      {"test", "(t) Run randomized test cases"},
      {"help", "(h) Print this message"},
      {"quit", "(q) Quit the program"}};

  std::cout << "commands:\n";
  for (auto &[cmd, desc] : cmds) {
    std::cout << "    ";
    std::cout << cmd << std::string(cmd_spc - int(cmd.size()), ' ') << desc << "\n";
  }
}
