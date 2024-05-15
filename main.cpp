#include "arcs.hpp"
#include "chords.hpp"
#include "coin.hpp"
#include "dyck_post.hpp"
#include "dyck_pre.hpp"
#include "main.hpp"
#include "poly.hpp"
#include "tree.hpp"
#include "util.hpp"

#include <exception>
#include <iostream>
#include <ranges>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

int main(int argc, const char *argv[]) {
  std::cout << "0 - triangulation\n"
            << "1 - chords\n"
            << "2 - arcs\n"
            << "3 - dyck path pre order\n"
            << "4 - coins\n"
            << "5 - dyck path post order\n\n";

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
    std::vector<std::string> tokens{split_string(input, ' ')};

    try {
      std::string cmd = tokens.at(0);
      if (cmd == "test" || cmd == "t") {
        test_expected_height_pre_order();
        test_conversion_dyck_path_pre_order();
      } else if (cmd == "5a") {
        // int length{get_num(tokens.at(1))};
        // plot_all_coin_stacks(length);
      } else if (cmd == "5r") {
        // int length{get_num(tokens.at(1))};
        // int count{tokens.size() == 2 ? 1 : get_num(tokens.at(3))};
        // plot_random_coin_stack(length, count);
      } else if (cmd == "5t") {
        test_conversion_dyck_path_post_order();
        test_expected_height_post_order();
      } else if (cmd == "5e") {
        // SKIP
        //----------------------------------
      } else if (cmd == "4a") {
        int length{get_num(tokens.at(1))};
        plot_all_coin_stacks(length);
      } else if (cmd == "4r") {
        int length{get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : get_num(tokens.at(3))};
        plot_random_coin_stack(length, count);
      } else if (cmd == "4t") {
        // SKIP
      } else if (cmd == "4e") {
        // SKIP
        //----------------------------------
      } else if (cmd == "3a") {
        int length{get_num(tokens.at(1))};
        plot_all_dyck_path(length);
      } else if (cmd == "3r") {
        int r{get_num(tokens.at(1))};
        int length{get_num(tokens.at(2))};
        int count{tokens.size() == 3 ? 1 : get_num(tokens.at(3))};
        plot_random_dyck_path(length, r, count);
      } else if (cmd == "3t") {
        // SKIP
      } else if (cmd == "3e") {
        int length{get_num(tokens.at(1))};
        int r{get_num(tokens.at(2))};
        Dyck dyck{get_random_dyck_path_pre_order(r, length)};
        flip_mountain(dyck);
        //----------------------------------
      } else if (cmd == "2a") {
        int num_of_points{get_num(tokens.at(1))};
        plot_all_arcs(num_of_points);
      } else if (cmd == "2r") {
        int num_of_points{get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : get_num(tokens.at(2))};
        plot_random_arcs(num_of_points, count);
      } else if (cmd == "2t") {
        test_conversion_arcs();
      } else if (cmd == "2e") {
        int num_of_points{get_num(tokens.at(1))};
        Arcs arcs{get_random_arcs(num_of_points)};
        exchage_arcs(arcs);
        //----------------------------------
        //-----------------------
      } else if (cmd == "1a") {
        int num_of_points{get_num(tokens.at(1))};
        plot_all_chords(num_of_points);
      } else if (cmd == "1r") {
        int num_of_points{get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : get_num(tokens.at(2))};
        plot_random_chords(num_of_points, count);
      } else if (cmd == "1t") {
        test_conversion_chords();
      } else if (cmd == "1e") {
        int num_of_points{get_num(tokens.at(1))};
        Chords chords{get_random_chords(num_of_points)};
        exchage_chords(chords);
        //----------------------------------
      } else if (cmd == "0a") {
        int num_of_sides{get_num(tokens.at(1))};
        plot_all_poly(num_of_sides);
      } else if (cmd == "0r") {
        int num_of_sides{get_num(tokens.at(1))};
        int count{tokens.size() == 2 ? 1 : get_num(tokens.at(2))};
        plot_random_poly(num_of_sides, count);
      } else if (cmd == "0t") {
        test_conversion_poly();
      } else if (cmd == "0f") {
        int num_of_sides{get_num(tokens.at(1))};
        Poly poly{get_random_poly(num_of_sides)};
        flip_and_plot(poly);
        //---------------------------------
      } else if (cmd == "quit" || cmd == "q") {
        break;
      } else if (cmd == "gen" || cmd == "g") {
        int deg{get_num(tokens.at(1))};
        int len{get_num(tokens.at(2))};
        std::string dyck_path{get_random_dyck_path_pre_order(deg, len)};
        std::cout << dyck_path << "\n";
        prev_dyck_path = dyck_path;
      } else if (cmd == "plot" || cmd == "p") {
        std::string dyck_path{tokens.size() == 2 ? tokens.at(1) : prev_dyck_path};
        auto tree{dyck_path_to_tree_pre_order(dyck_path)};
        store_tree_into_file(tree, TREE_OUTPUT_FILENAME);
        free_tree(tree);
        plot_tree(TREE_OUTPUT_FILENAME);
      } else if (cmd == "all" || cmd == "a") {
        int num_of_internal_nodes = get_num(tokens.at(1));
        plot_all_trees(num_of_internal_nodes);
      } else if (cmd == "decode" || cmd == "d") {
        std::string dyck_path{tokens.at(1)};
        auto tree{dyck_path_to_tree_pre_order(dyck_path)};
        store_tree_into_file(tree, TREE_OUTPUT_FILENAME);
        free_tree(tree);
        plot_tree(TREE_OUTPUT_FILENAME);
      } else if (cmd == "encode" || cmd == "e") {
        std::string file{tokens.at(1)};
        auto tree{get_tree_from_file(file)};
        std::string dyck_path{tree_to_dyck_path_pre_order(tree)};
        std::cout << dyck_path << "\n";
        free_tree(tree);
        prev_dyck_path = dyck_path;
      } else if (cmd == "save" || cmd == "s") {
        std::string dyck_path{tokens.at(1)};
        std::string file{tokens.at(2)};
        auto tree{dyck_path_to_tree_pre_order(dyck_path)};
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
