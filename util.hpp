#include <string>
#include <vector>

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
 * Print a vector to console
 *
 * @param vec: The vector in question
 */
template <typename T> void print_vector(const T &vec);

/**
 * Print a C array to console
 *
 * @param arr: The C-style array in question
 * @param len: Length of the C-style array
 */
template <typename T> void print_c_array(const T *arr, int len);

/**
 * Get n^{th} Catalan number. Throw an exception if int overflow.
 *
 * @param n: n^{th} Catalan number
 *
 * @return n^{th} Catalan number
 */
int get_catalan(int n);
