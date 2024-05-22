#include "base_tree.hpp"
#include "tree.hpp"
#include <cassert>
#include <functional>
#include <memory>
#include <ranges>

std::unique_ptr<Tree> Tree::get_from_bitstring(const std::string &bitstring) {
  auto tree{BaseTree::get_from_bitstring(bitstring)};
  return Tree::of(std::move(tree));
}

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
  return std::make_unique<Tree>(std::move(base_tree));
}

std::unique_ptr<CoinStack> Tree::to_coin_stack() {
  return this->to_dyck_pre()->to_coin_stack();
}

std::unique_ptr<DyckPreMirrored> Tree::to_dyck_pre_mirrored() {
  std::string encoded_result{};
  std::function<void(const Node *)> encode = [&](const Node *cur_node) {
    int zero{int(encoded_result.size())};
    for (const auto &next_node : cur_node->children | std::ranges::views::reverse) {
      encoded_result += "1";
      encode(next_node.get());
    }
    if (cur_node->is_internal_node()) {
      encoded_result[zero] = '0';
    }
  };
  encode(this->root.get());
  if (!DyckPreMirrored::is_valid(encoded_result)) {
    std::cerr << "Error: This tree is not a full k-ary tree\n";
    throw std::invalid_argument("");
  }
  return std::make_unique<DyckPreMirrored>(encoded_result);
}

std::unique_ptr<DyckPre> Tree::to_dyck_pre() {
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
  if (!DyckPre::is_valid(encoded_result)) {
    std::cerr << "Error: This tree is not a full k-ary tree\n";
    throw std::invalid_argument("");
  }
  return std::make_unique<DyckPre>(encoded_result);
}

std::unique_ptr<Arcs> Tree::to_arcs() {
  auto chords{this->to_chords()};
  auto arcs{chords->to_arcs()};
  return arcs;
}

std::unique_ptr<Chords> Tree::to_chords() {
  Graph chords;
  int id{};

  std::function<int(const Node *)> build = [&](const Node *cur_node) {
    if (cur_node->is_leaf()) {
      return id++;
    }
    assert(cur_node->child_count() == 2);
    int assigned_id{id++};
    int left_id{build(cur_node->children[0].get())};
    int right_id{build(cur_node->children[1].get())};
    chords.push_back({left_id, right_id});
    return assigned_id;
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
