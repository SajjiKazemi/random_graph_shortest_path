# Finding the Shortest Path Between Any Two Nodes of Randomly Automatically-Generating Graphs

## Overview

This project is designed and implemented to find the shortest path between any two nodes of a randomly generated graph using the breadth-first research technique. It marks the third phase of a broader project aimed at identifying a vertex cover for streets with intersections. The ultimate goal is to determine the vertex cover of a randomly generated graph, addressing the Vertex Cover problem, a specific type of optimization problem in graph theory. For a brief understanding of the vertex cover concept, refer to [this link](https://en.wikipedia.org/wiki/Vertex_cover#:~:text=In%20graph%20theory%2C%20a%20vertex,every%20edge%20of%20the%20graph).

In this project, the input_parser.py output format aligns with the input requirements of find_shortest_path.cpp. Additionally, rgen.cpp generates random input for the input_parser.py script. Notably, main.cpp utilizes Inter-Process Communication (IPC) to establish a seamless link between the output of the random input generator and input_parser.py, and the output of input_parser.py to the input of the find_shortest_path.cpp program.

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/SajjiKazemi/random_graph_shortest_path.git
```

## Dependencies
Create a virtual environment using the provided my_env.yml file:

```bash
conda env create -f my_env.yml
```
Activate the environment:

```bash
conda activate my_env
```

## Usage

To run the project, inside the project directory, create the necessary directory:

```bash
mkdir build
```

and execute the following command line:

```bash
cmake ../
```

and then:

```bash
make install
```

Navigate the following directory:

```bash
cd ./run/bin
```

Run the executable file for main.cpp. Optional arguments can be provided as follows:


```bash
 ./main -s 5 -n 4 -l 5
```

Explanation of optional arguments:

-s k: Integer ≥ 2. Specifies the number of streets as a random integer in [2, k]. Default: k = 10.

-n k: Integer ≥ 1. Specifies the number of line-segments in each street as a random integer in [1, k]. Default: k = 5.

-l k: Integer ≥ 5. The process waits a random number w seconds, where w is in [5, k], before generating the next (random) input. Default: k = 5.

-c k: Integer ≥ 1. Generates (x, y) coordinates such that every x and y value is in the range [−k, k]. For example, if k = 15, all coordinate values will be integers between −15 and 15. Default: k = 20.

Upon running the project, randomly generated graphs with their characteristics will be displayed. The 's' command, followed by a space, a vertex ID, another space, and a second vertex ID, can be used to calculate and display the shortest path between two nodes corresponding to those vertex IDs.

## Contact
For any questions or feedback, feel free to reach out:

Email: sajjikazemi@uwaterloo.ca

LinkedIn: www.linkedin.com/in/sajjadkazemi
