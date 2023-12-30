cmake_minimum_required(VERSION 2.8...3.13)
project(random_graph_shortest_path)

## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

option(WITH_ASAN "Enabled AddressSanitizer to detect memory faults" OFF)
if(WITH_ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined" )
endif()


# create the main executable
## add additional .cpp files if needed
add_executable(find_shortest_path find_shortest_path.cpp )
add_executable(main main.cpp )
add_executable(rgen rgen.cpp helpers.cpp MyGraph.cpp BFStree.cpp)

# create the executable for tests
add_executable(tests test.cpp helpers.cpp MyGraph.cpp BFStree.cpp)
install(PROGRAMS input_parser.py DESTINATION bin)
install(TARGETS find_shortest_path main RUNTIME DESTINATION bin)
