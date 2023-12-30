#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include "helpers.h"

int main(int argc, char **argv)
{
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;

    int opt;
    while ((opt = getopt(argc, argv, "s:n:l:c:")) != -1)
    {
        switch (opt)
        {
        case 's':
            s = atoi(optarg);
            if (s < 2)
            {
                std::cerr << "Error: Invalid option" << std::endl;
                return 1;
            }
            break;
        case 'n':
            n = atoi(optarg);
            if (n < 1)
            {
                std::cerr << "Error: Invalid option" << std::endl;
                return 1;
            }
            break;
        case 'l':
            l = atoi(optarg);
            if (l < 5)
            {
                std::cerr << "Error: Invalid option" << std::endl;
                return 1;
            }
            break;
        case 'c':
            c = atoi(optarg);
            if (c < 1)
            {
                std::cerr << "Error: Invalid option" << std::endl;
                return 1;
            }
            break;
        default:
            std::cerr << "Error: Invalid option" << std::endl;
            return 1;
        }
    }

    std::vector<std::string> command_lines;
    std::vector<std::string> streets_name;
    std::vector<std::string> old_streets_name;
    std::vector<std::vector<std::pair<int, int>>> streets_coordinates;
    int waiting_time = 0;
    double count = 0;

    while (!std::cin.eof())
    {
        
        int trial_number = 0;
        bool valid = false;
        while (trial_number < 25 && valid == false)
        {
            streets_name.clear();
            streets_coordinates.clear();
            
            int street_numbers = rgen_needs::rgen(s, 2);
            for (int i=0; i<street_numbers; i++)
            {
                streets_name.push_back("street_" + std::to_string(i));
            }

            waiting_time = rgen_needs::rgen(l, 5);

            std::vector<int> line_segments;
            for (int i=0; i<street_numbers; i++)
            {
                line_segments.push_back(rgen_needs::rgen(n, 1));
            }

            std::vector<std::pair<int, int>> coordinates;

            for (int i=0; i<street_numbers; i++)
            {
                for (int j=0; j<line_segments[i]+1; j++)
                {
                    int x = rgen_needs::rgen(c, -c);
                    int y = rgen_needs::rgen(c, -c);
                    coordinates.push_back(std::make_pair(x, y));
                }
                streets_coordinates.push_back(coordinates);
                coordinates.clear();
            }

            valid = rgen_needs::validate_graph(streets_coordinates);
            trial_number++;
        }

        if (trial_number >= 25)
        {
            std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << std::endl;
            return 1;
        }
        else
        {
            command_lines = rgen_needs::create_command_lines(streets_name, streets_coordinates, old_streets_name, count);
            count++;
            old_streets_name = streets_name;
        }

        // for (auto& line : command_lines)
        // {
        //     std::cout << line;
        //     std::cout << std::endl;
        // }
        sleep(waiting_time);
    }

    return 0;
    
}