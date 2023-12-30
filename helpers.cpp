#include "helpers.h"
#include "MyGraph.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

namespace line_parser{

    std::string get_command(std::string line, MyGraph &graph){

        std::regex vertice_define_pattern(R"(V (\d+))");
        std::regex edge_define_pattern(R"(E \{<\d+,\d+>(,<\d+,\d+>)*\})");
        std::regex shortest_path_pattern(R"(s (\d+) (\d+))");
        std::regex E_only_pattern(R"(E\s.*)");
        //std::regex s_only_pattern(R"(s\s.*)");
        std::string command;

        if(std::regex_match(line, vertice_define_pattern))
        {
            command = "V";
        }
        else if(std::regex_match(line, edge_define_pattern))
        {
            if (graph.getSize() == 0)
            {
                command = "Error";
                std::cout << "Error: no vertices defined" << std::endl;
            }
            else if (graph.getSize() != 0 && graph.edges.size() != 0)
            {
                command = "No Command";
            }
            else if (graph.getSize() != 0 && graph.edges.size() == 0)
            {
                if (graph.triedToSetEdges == true)
                {
                    command = "No Command";
                }
                else if (graph.triedToSetEdges == false)
                {
                    command = "E";
                }
            }
        }        
        else if(std::regex_match(line, shortest_path_pattern))
        {
            if (graph.edges.size() == 0)
            {
                command = "Error";
                std::cout << "Error: no edges defined" << std::endl;
            }
            else if (graph.edges.size() != 0)
            {
                command = "s";
            }
        }
        else if(std::regex_match(line, E_only_pattern))
        {
            if (graph.edges.size() !=0)
            {
                return "No command";        
            }
            else if (graph.edges.size() == 0)
            {
                graph.setTriedToSetEdges();
                std::cout << "Error: invalid command" << std::endl;
            }
            
        }
        return command;
    }    

    void analyze_command(std::string command, std::string line, MyGraph &graph)
    {
        std::vector<std::string> result;
        if(command == "V")
        {
            std::istringstream input(line);
            std::string prefix;
            int number;
            
            graph.resetEverything();
            input >> prefix >> number;
            graph.setNoVertices(number);
        }
        else if (command == "E")
        {
            std::map<int, std::pair<int, int>> edges;
            std::regex pattern(R"(<(\d+),(\d+)>)");
            std::sregex_iterator iter(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            int i = 0;
            while (iter != end) 
            {
                i++;
                auto match = *iter++;
                if (std::stoi(match[1]) > graph.getSize() || std::stoi(match[2]) > graph.getSize())
                {
                    std::cout << "Error: edge is not valid" << std::endl;
                    return;
                }
                edges[i] = std::make_pair(std::stoi(match[1]), std::stoi(match[2]));
            }
            graph.setEdges(edges);
        }
        else if (command == "s")
        {
            std::istringstream input(line);
            std::string prefix;
            int start;
            int end;

            input >> prefix >> start >> end;
            graph.findShortestPath(start, end);
            for(std::vector<int>::size_type i = 0; i < graph.getShortestPath().size(); i++)
            {
                if (i < graph.getShortestPath().size() - 1)
                {
                    std::cout << graph.getShortestPath()[i] << "-";
                }
                else
                {
                    std::cout << graph.getShortestPath()[i] << std::endl;
                }
            }
        }
        else if (command == "No Command" || command == "Error")
    {
        return;
    }
    }

}

namespace rgen_needs
{
    int rgen(int maximum, int minimum)
        {
            int random_number;
            std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
            char my_random_char;
            urandom.read(&my_random_char, sizeof(my_random_char));
            random_number = static_cast<int>(static_cast<unsigned char>(my_random_char));
            random_number = random_number % (maximum - minimum + 1) + minimum;
            urandom.close();
            return random_number;
        }

    bool validate_graph(std::vector<std::vector<std::pair<int,int>>> streets_coordinates)
    {
        if (streets_coordinates.size() == 0)
        {
            return false;
        }
        for (std::vector<int>::size_type i = 0; i < streets_coordinates.size(); i++)
        {
            if (streets_coordinates[i].size() < 2)
            {
                return false;
            }
            
            std::vector<std::pair<int,int>> coordinates = streets_coordinates[i];
            for (std::vector<int>::size_type j = 0; j < coordinates.size(); j++)
            {
                if (j == coordinates.size() - 1)
                {
                    break;
                }
                if (coordinates[j].first == coordinates[j+1].first && coordinates[j].second == coordinates[j+1].second)
                {
                    return false;
                }
            if (coordinates[j].first == coordinates[j+1].second && coordinates[j].second == coordinates[j+1].first)
                {
                    return false;
                }
            if (coordinates[j].first == coordinates[j].second)
                {
                    return false;
                }
        }
        }
        return true;
    }

    std::vector<std::string> create_command_lines(std::vector<std::string> streets_name, 
    std::vector<std::vector<std::pair<int, int>>> streets_coordinates,
    std::vector<std::string> old_streets_name, int count)
    {
        std::vector<std::string> command_lines;
        std::string command;
        if (count != 0)
        {
            for (std::vector<int>::size_type i = 0; i < old_streets_name.size(); i++)
            {
                command = "rm " + old_streets_name[i];
                command_lines.push_back(command);
            }
        }
        for (std::vector<int>::size_type i = 0; i < streets_name.size(); i++)
        {
            command = "add " + streets_name[i];

            for (std::vector<int>::size_type j = 0; j < streets_coordinates[i].size(); j++)
            {
                command = command + " (" + std::to_string(streets_coordinates[i][j].first) + "," + std::to_string(streets_coordinates[i][j].second) + ")"; 
            } 
            command_lines.push_back(command);
        }

        command_lines.push_back("gg");
        return command_lines;
    }
    
    const char* get_full_command(const char* command)
        {
            const char* full_command = (std::string(command)).c_str();
            return full_command;
        }

}