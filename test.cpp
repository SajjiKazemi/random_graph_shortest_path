/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "helpers.h"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

TEST_CASE("Check rgen") 
{
    int random_number = rgen_needs::rgen(7,9);
    CHECK(random_number >= 7);
    CHECK(random_number <= 9);
}

std::string exec(const std::vector<std::string>& commands) {
    std::array<char, 128> buffer;
    std::string result;

    for (const auto& cmd : commands) {
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("popen() failed for command: " + cmd);
        }

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        pclose(pipe);
    }

    return result;
}

TEST_CASE("Test the rgen executable") 
{
    std::vector<std::string> commands;
    std::string command = "./rgen -s 15 -l -1 -n 21 2>&1";
    commands.push_back(command);
    std::string output = exec(commands);

    // Use doctest assertions to check the output
    CHECK(output == "Error: Invalid option\n");
}

TEST_CASE("Test the pyfile executable") 
 {
    std::vector<std::string> commands;
    std::string command = "make pyfile < ../input.txt";
    commands.push_back(command);
    std::string output = exec(commands);
    CHECK(output == "Error: You did not obey the requested format");

 }