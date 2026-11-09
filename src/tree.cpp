#include "base_tree.hpp"
#include "tree.hpp"
#include <cassert>
#include <functional>
#include <memory>
#include <ranges>

std::unique_ptr<Tree> Tree::get_from_traversal(const std::string &traversal) {
  auto tree{BaseTree::get_from_traversal(traversal)};
  return Tree::of(std::move(tree));
}

std::unique_ptr<Tree> Tree::get_from_file(std::string file) {
  auto tree{BaseTree::get_from_file(file)};
  return Tree::of(std::move(tree));
}

std::unique_ptr<Tree> Tree::get_random(int deg, int num_of_nodes) {
  auto tree = BaseTree::get_random(deg, num_of_nodes);
  return Tree::of(std::move(tree));
}

std::unique_ptr<Tree> Tree::of(std::unique_ptr<BaseTree> base_tree) {
  auto tree{std::make_unique<Tree>(std::move(base_tree))};
  tree->type = TreeType::Full;
  return tree;
}

std::unique_ptr<CoinStack> Tree::to_coin_stack() {
  return this->to_dyck()->to_coin_stack();
}

std::unique_ptr<Dyck> Tree::to_dyck() {
  std::string encoded_result{};
  std::function<void(const Node *)> encode = [&](const Node *cur_node) {
    int zero{};
    for (const auto &next_node : cur_node->children) {
      zero = encoded_result.size();
      encoded_result += "1";
      encode(next_node.get());
    }
    encoded_result[zero] -= cur_node->is_internal_node();
  };

  encode(root.get());
  if (!Dyck::is_valid(encoded_result)) {
    std::cerr << "Error: This tree is not a full k-ary tree\n";
    throw std::invalid_argument("");
  }
  return std::make_unique<Dyck>(encoded_result);
}

std::unique_ptr<Arcs> Tree::to_arcs() {
  auto chords{this->to_chords()};
  auto arcs{chords->to_arcs()};
  return arcs;
}

std::unique_ptr<Chords> Tree::to_chords() {
  Graph chords;
  int id{};

  std::function<void(const Node *)> build = [&](const Node *cur_node) {
    if (cur_node->is_leaf()) {
      return;
    }
    assert(cur_node->child_count() == 2);
    int a{id++};
    build(cur_node->children[0].get());
    int b{id++};
    chords.push_back({a, b});
    build(cur_node->children[1].get());
  };

  build(this->root.get());
  if (!Chords::is_valid(chords)) {
    std::cerr << "Error: tree to chords graph failed.\n";
    throw std::invalid_argument("");
  }
  return std::make_unique<Chords>(chords);
}

std::unique_ptr<Poly> Tree::to_poly() {
  Graph poly;

  int count{};
  enum Dir { Left, Right, None };

  std::function<int(const Node *, Dir)> build = [&](const Node *cur_node, Dir dir) {
    if (cur_node->is_leaf()) {
      return ++count - (Dir::Left == dir);
    }
    assert(int(cur_node->children.size()) == 2);
    int l{build(cur_node->children[0].get(), Dir::Left)};
    int r{build(cur_node->children[1].get(), Dir::Right)};
    poly.push_back({l, r});
    return dir == Dir::Left ? l : r;
  };

  build(this->root.get(), Dir::None);
  std::ranges::reverse(poly);
  if (!Poly::is_valid(poly)) {
    std::cerr << "Error: tree to polygon triangulation failed.\n";
    throw std::invalid_argument("");
  }
  return std::make_unique<Poly>(poly);
}

std::unique_ptr<Bracketing> Tree::to_bracketing() {
  std::string res;

  std::function<void(const Node *)> build = [&](const Node *cur_node) {
    if (cur_node->is_leaf()) {
      res += "x";
      return;
    }
    for (const auto &next_node : cur_node->children) {
      bool inner{next_node->is_internal_node()};
      res += inner ? "(" : "";
      build(next_node.get());
      res += inner ? ")" : "";
    }
  };

  build(this->root.get());
  return std::make_unique<Bracketing>(res);
}

void Tree::show_all() {
  this->to_file();
  this->to_dyck()->to_file();
  this->to_poly()->to_file();
  this->to_bracketing()->to_file();
  this->to_arcs()->to_file();
  this->to_chords()->to_file();
  this->to_coin_stack()->to_file();

  std::string window{std::format(
      "./visualizer/bin/python3 ./visualizer/{} {} {} {} {} {} {} {}",
      Tree::_SHOW_SCRIPT, BaseTree::_DEFAULT_PREFIX_FILE, Dyck::_DEFAULT_PREFIX_FILE,
      Poly::_DEFAULT_PREFIX_FILE, Bracketing::_DEFAULT_PREFIX_FILE,
      Arcs::_DEFAULT_PREFIX_FILE, Chords::_DEFAULT_PREFIX_FILE,
      CoinStack::_DEFAULT_PREFIX_FILE)};
  std::cout << "Close the window to return.\n";
  if (std::system(window.c_str())) {
    std::cerr << "The show window could not be started.\n";
  }
}
