#pragma once

#include <string>
#include <vector>

/**
 * Print usage
 */
void print_usage();

/**
 * Init
 */
bool init();

/**
 * Print help for commands
 */
void print_hint(const std::vector<std::string> &tokens);

/**
 * This form a command names, with "tri" for "triangulation"
 */
std::string form_name(const std::vector<std::string> &tokens);
