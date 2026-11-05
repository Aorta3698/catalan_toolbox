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
 * Return the full name of a word
 */
std::string form_name(const std::string &word);
