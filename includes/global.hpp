#pragma once

#include "xoshiro256.hpp"

#include <random>
#include <string>

// seed generator once
inline std::random_device rd;
inline xoshiro256ss g_256ss(rd());

inline const static std::string Arcs_PLOT_SCRIPT = "plot-arcs.py";
inline const static std::string CHORDS_PLOT_SCRIPT = "plot-chords.py";
inline const static std::string POLY_PLOT_SCRIPT = "plot-poly.py";
inline const static std::string TREE_PLOT_SCRIPT = "plot-tree.py";
inline const static std::string ARCS_PLOT_SCRIPT = "plot-arcs.py";
inline const static std::string DYCK_PLOT_SCRIPT = "plot_dyck_path.py";
inline const static std::string COIN_PLOT_SCRIPT = "plot_coins.py";

using Edge = std::array<int, 2>;
using Graph = std::vector<Edge>;