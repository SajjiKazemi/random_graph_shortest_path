#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>


// Entry point of process input_parser
int input_parser_process(void) 
{
    //run the input_parser executable
    char* args[3];
    args[0] = "python";
    args[1] = "./input_parser.py";
    args[2] = nullptr;
    execvp(args[0], args);

    std::cerr << "Error: input_parser exec failed" << std::endl;
    return 1;
}

// Entry point of process rgen
int rgen_process(int argc, char **argv) 
{

    //run the rgen executable
    char* args[argc];
    args[0] = "./rgen";
    for (int i = 1; i < argc; i++)
    {
        args[i] = argv[i];
    }
    args[argc] = nullptr;
    execv(args[0], args);

    std::cerr << "Error: rgen exec failed" << std::endl;
    return 1;
}

int find_shortest_path_process(void) 
{
    //run the find_shortest_path executable
    char* args[3];
    args[0] = "./find_shortest_path";
    args[1] = nullptr;
    execvp(args[0], args);
    std::cerr << "Error: find_shortest_path exec failed" << std::endl;
    return 1;
}
    

int main (int argc, char **argv) 
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

    //create a vector to store the children's pid
    std::vector<pid_t> children;
    //create a pipe
    int rgen_to_input_parser[2];
    int input_parser_to_find_shortest_path[2];

    pipe(input_parser_to_find_shortest_path);
    pipe(rgen_to_input_parser);
    

    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        dup2(input_parser_to_find_shortest_path[1], STDOUT_FILENO);
        close(input_parser_to_find_shortest_path[0]);
        close(input_parser_to_find_shortest_path[1]);
        dup2(rgen_to_input_parser[0], STDIN_FILENO);
        close(rgen_to_input_parser[0]);
        close(rgen_to_input_parser[1]);

        return input_parser_process();
    }
    else if (child_pid < 0)
    {
        std::cerr << "Error: input_parser fork failed" << std::endl;
        return 1;
    }
    children.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0)
    {
        dup2(input_parser_to_find_shortest_path[0], STDIN_FILENO);
        close(input_parser_to_find_shortest_path[0]);
        close(input_parser_to_find_shortest_path[1]);
        close(rgen_to_input_parser[0]);
        close(rgen_to_input_parser[1]);

        //std::cout << STDIN_FILENO << std::endl;

        return find_shortest_path_process();
    }
    else if (child_pid < 0)
    {
        std::cerr << "Error: find_shortest_path fork failed" << std::endl;
        return 1;
    }
    children.push_back(child_pid);

    pid_t rgen_pid = fork();
    if (rgen_pid == 0)
    {
        dup2(rgen_to_input_parser[1], STDOUT_FILENO);
        close(rgen_to_input_parser[0]);
        close(rgen_to_input_parser[1]);
        close(input_parser_to_find_shortest_path[0]);
        close(input_parser_to_find_shortest_path[1]);

        return rgen_process(argc, argv);
    }
    else if (rgen_pid < 0)
    {
        std::cerr << "Error: rgen fork failed" << std::endl;
        return 1;
    }
    //children.push_back(rgen_pid);

    pid_t driver_pid = fork();
    if (driver_pid == 0)
    {
        dup2(input_parser_to_find_shortest_path[1], STDOUT_FILENO);
        close(input_parser_to_find_shortest_path[0]);
        close(input_parser_to_find_shortest_path[1]);
        close(rgen_to_input_parser[0]);
        close(rgen_to_input_parser[1]);

        while (!std::cin.eof())
        {
            std::string line;
            std::getline(std::cin, line);
            if (line.size() > 0)
            {
                std::cout << line << std::endl;
            }
        }

        return 0;
    }
    else if (driver_pid < 0)
    {
        std::cerr << "Error: driver fork failed" << std::endl;
        return 1;
    }

    close(input_parser_to_find_shortest_path[0]);
    close(input_parser_to_find_shortest_path[1]);
    close(rgen_to_input_parser[0]);
    close(rgen_to_input_parser[1]);

    int status;
    while (true)
    {
        if (waitpid(driver_pid, &status, WNOHANG))
        {
            kill(rgen_pid, SIGTERM);
            int status;
            waitpid(rgen_pid, &status, 0);
            break;
        }

        if (waitpid(rgen_pid, &status, WNOHANG))
        {
            kill(driver_pid, SIGTERM);
            int status;
            waitpid(driver_pid, &status, 0);
            break;
        }
    }

    for (auto const &child : children)
    {
        kill(child, SIGTERM);
        int status;
        waitpid(child, &status, 0);
    }
    return 0;
}