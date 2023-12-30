#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "MyGraph.h"

namespace line_parser {
    // get the command from a line
    std::string get_command(std::string line, MyGraph &graph);
    // split a string into a vector of strings
    void analyze_command(std::string command, std::string line, MyGraph &graph);
}

namespace rgen_needs
{
    int rgen(int maximum, int minimum);
    bool validate_graph(std::vector<std::vector<std::pair<int, int>>> streets_coordinates);
    std::vector<std::string> create_command_lines(std::vector<std::string> streets_name,
    std::vector<std::vector<std::pair<int, int>>> streets_coordinates,
    std::vector<std::string> old_streets_name, int count);
    const char* get_full_command(const char* command);
}

#endif