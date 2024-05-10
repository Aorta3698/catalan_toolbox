#pragma once

#include "global.hpp"

#include <string>

/**
 * Plot a coin_stack
 *
 * @param dyck_path: a dyck path
 * @param file:      file to store info needed for graphing it
 */
void plot_coin_stack(const Dyck dyck_path, std::string file);

/**
 * Plot all coin_stack graph with bottom layer having n coins
 *
 * @param n:  Number of coins on the bottom. Between 1 to 4, inclusive.
 */
void plot_all_coin_stacks(int n);

/**
 * Plot `count` number of random coin stacks with n coins on the bottom
 *
 * @param num_of_sides:   Number of coins on the bottom
 * @param count:          Number of random coin stack plots
 */
void plot_random_coin_stack(int n, int count);
