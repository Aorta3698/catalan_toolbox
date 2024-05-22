#pragma once

#include "global.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace Util {
static std::array<mint, 10'001> fact;

/**
 * The necessary init for storing factorial.
 *
 */
void setup_comb();

/**
 * Compute n choose k
 *
 * @param n:  The top number
 * @param k:  The bottom number
 *
 * @return n choose k
 */
mint combinations(int n, int k);

/**
 * Split str by the given delimiter
 *
 * @param str:       Input string
 * @param delimiter: A delimiter of char type
 *
 * @return A vector containing the splitted string
 */
std::vector<std::string> split_string(std::string str, char delimiter);

/**
 * Convert a string to a positive integer. Throw an exception if
 * the input is not fully processed or if it is non-positive.
 *
 * @param str:   A number in string
 *
 * @return The converted number of int type
 */
int get_num(std::string str);

/**
 * Trim the whitespaces from both ends of a string.
 * It modifies it in place.
 *
 * @param str:   Input string
 *
 * @return A vector containing k random numbers
 */
std::string &strip(std::string &str);

/**
 * Get n^{th} Catalan number. Throw an exception if int overflow.
 *
 * @param n: n^{th} Catalan number
 *
 * @return n^{th} Catalan number
 */
int get_catalan(int n);

/**
 * Rotate coordinate (x, y) around origin by deg counter-clockwise
 * From: https://en.wikipedia.org/wiki/Rotation_matrix#In_two_dimensions
 *
 * @param x  : x coordinate
 * @param y  : y coordinate
 * @param deg: degree in 360 format
 *
 * @return an array of size 2 containing the rotated coordinates
 */
std::array<double, 2> rotate(double x, double y, double deg);

/**
 * Fork a child process and plot the input graph with the given script
 *
 * @param script  : Python visualizer script to executa
 * @param file    : The input file to the script
 */
void plot(std::string script, std::string file);

/**
 * Print a vector to console
 *
 * @param vec: The vector in question
 */
template <typename T> void print_vector(const T &vec) {
  std::copy(vec.cbegin(), vec.cend(),
            std::ostream_iterator<typename T::value_type>(std::cerr, ", "));
  std::cerr << "\n";
}

/**
 * Print a vector of vector to console
 *
 * @param vec: The vector in question
 */
template <typename T> void print_vvector(const T &vec) {
  std::for_each(vec.cbegin(), vec.cend(),
                Util::print_vector<typename T::value_type>);
}

/**
 * Print a C array to console
 *
 * @param arr: The C-style array in question
 * @param len: Length of the C-style array
 */
template <typename T> void print_c_array(const T *arr, int len) {
  for (int i{}; i < len; ++i) {
    std::cerr << arr[i] << " ";
  }
  std::cerr << "\n";
}
} // namespace Util
