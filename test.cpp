/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "helpers.h"
#include <unistd.h>
#include <iostream>

TEST_CASE("Check rgen") 
{
    std::vector<int> result = random_number::rgen();
    CHECK(result.size() == 4);
    CHECK(result[0] >= 2);
    CHECK(result[0] <= 10);
    CHECK(result[1] >= 1);
    CHECK(result[1] <= 5);
    CHECK(result[2] >= 5);
    CHECK(result[2] <= 5);
    CHECK(result[3] >= -20);
    CHECK(result[3] <= 20);

    result = random_number::rgen(100, 10, 10, 100);
    CHECK(result.size() == 4);
    CHECK(result[0] >= 2);
    CHECK(result[0] <= 100);
    CHECK(result[1] >= 1);
    CHECK(result[1] <= 10);
    CHECK(result[2] >= 5);
    CHECK(result[2] <= 10);
    CHECK(result[3] >= -100);
    CHECK(result[3] <= 100);
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}

TEST_CASE("Test the rgen executable") 
{
    std::string command = "./rgen -s 15 -l -1 -n 21 2>&1";
    std::string output = exec(command.c_str());

    // Use doctest assertions to check the output
    CHECK(output == "Error: Invalid option\n");
}