#!/usr/bin/env python3
# A simple unit test example. Replace by your own tests

import sys
import unittest
from unittest.mock import patch
import io
from helpers import *
from vertexCover import *


class MyTest(unittest.TestCase):

    def test_print_vertices_command(self):
        expected_output1 = "V 10"
        expected_output2 = "E {<1,5>,<2,5>,<3,5>,<5,8>,<4,8>,<6,8>,<7,8>,<9,8>,<10,8>}"
        vertex_cover = vertexCover()
        line = 'add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)'
        cmd, street_name, vertices = parse_input(line)
        vertex_cover.add_street(street_name, vertices)
        line = 'add "King Street S" (4,2) (4,8)'
        cmd, street_name, vertices = parse_input(line)
        vertex_cover.add_street(street_name, vertices)
        line = 'add "Davenport Road" (1,4) (5,8)'
        cmd, street_name, vertices = parse_input(line)
        vertex_cover.add_street(street_name, vertices)
        vertex_cover.update_vertices()
        
        with patch('sys.stdout', new=io.StringIO()) as fake_out:
            vertex_cover.print_vertices_command()
            self.assertEqual(fake_out.getvalue().strip(), expected_output1)

        with patch('sys.stdout', new=io.StringIO()) as fake_out:
            vertex_cover.print_edges_command()
            self.assertEqual(fake_out.getvalue().strip(), expected_output2)
        


if __name__ == '__main__':
    unittest.main()
