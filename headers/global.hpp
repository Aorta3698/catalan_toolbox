#include "xoshiro256.hpp"

#include <random>
#include <string>

// seed generator once
inline std::random_device rd;
inline xoshiro256ss g_256ss(rd());

inline const static std::string POLY_PLOT_SCRIPT = "plot-poly.py";
inline const static std::string TREE_PLOT_SCRIPT = "plot-tree.py";
