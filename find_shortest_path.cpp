#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include "helpers.h"
#include "MyGraph.h"

int main(int argc, char** argv) {

    // read from stdin until EOF
    MyGraph graph;
    std::regex pattern("s\\s\\d+\\s\\d+");
    while (!std::cin.eof()) 
    {

        std::string line;
        std::getline(std::cin, line);
        if (!std::regex_match(line, pattern))
        {
            std::cout << line << std::endl;
        } 

        // get the command from the line
        std::string command = line_parser::get_command(line, graph);
        line_parser::analyze_command(command, line, graph);
    }
}
