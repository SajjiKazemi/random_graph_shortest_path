#include <iostream>
#include <unistd.h>
#include <vector>

// Entry point of process input_parser
int input_parser_process(void) 
{
    //run the input_parser executable
    char* args[3];
    args[0] = "python";
    args[1] = "input_parser.py";
    args[2] = nullptr;
    execvp(args[0], args);

    std::cerr << "Error: exec failed" << std::endl;
    return 1;
}

// Entry point of process rgen
int rgen_process(void) 
{
    std::vector<std::string> lines;
    lines.push_back("add \"Weber Street\" (2,-1) (2,2) (5,5) (5,6) (3,8)");
    lines.push_back("add \"King Street S\" (4,2) (4,8)");
    lines.push_back("add \"Davenport Road\" (1,4) (5,8)");
    lines.push_back("gg");
    for (auto& line : lines)
    {
        std::cout << line << std::endl;
    }
    
    return 0;
}
    

int main (void) {
    std::vector<pid_t> children;
    //create a pipe
    int rgen_to_input_parser[2];
    int input_parser_to_find_shortest_path[2];
    int driver_to_find_shortest_path[2];

    pipe(rgen_to_input_parser);
    pid_t rgen_pid = fork();
    if (rgen_pid == 0)
    {
        //redirect stdout to pipe
        dup2(rgen_to_input_parser[0], STDIN_FILENO);
        //close unused pipe
        close(rgen_to_input_parser[0]);
        close(rgen_to_input_parser[1]);
        //execute input_parser

        return input_parser_process();
    }
    else if (rgen_pid < 0)
    {
        std::cerr << "Error: fork failed" << std::endl;
    }
    else
    {
        dup2(rgen_to_input_parser[1], STDOUT_FILENO);
        close(rgen_to_input_parser[0]);
        close(rgen_to_input_parser[1]);

        //execute rgen

        return rgen_process();
    }
    
    


    return 0;
}
