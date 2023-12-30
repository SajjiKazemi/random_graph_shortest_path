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

namespace random_number
{
    std::vector<int> rgen(int s = 10, int n = 5, int l = 5, int c = 20);
}

#endif