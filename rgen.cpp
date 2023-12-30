#include <iostream>
#include <unistd.h>
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

    std::vector<int> result = random_number::rgen(s, n, l, c);

    std::cout << "result: ";
    for (std::vector<int>::size_type i = 0; i < result.size(); i++)
    {
        std::cout << result[i] << " ";
    }
}