import sys
import re

from helpers import *

class vertexCover:
    def __init__(self):
        self.streets = {}
        self.vertices = {}
        self.intersections = {}
        self.edges = []
        self.segments = {}
    
    def add_street(self, name: str, vertices_str: list):
        vertices = []
        for vertex in vertices_str:
            try:
                x_arg = float(re.split("[(]", (re.split(",",vertex)[0]))[1])
                y_arg = float(re.split("[)]", (re.split(",",vertex)[1]))[0])
                vertices.append((x_arg, y_arg))
            except Exception as e:
                print('Error: You did not obey the requested format', file=sys.stderr)
                return
        self.streets[name] = vertices

    def mod_street(self, name: str, vertices_str: list):
        try:
            del self.streets[name]
            vertices = []
            for vertex in vertices_str:
                x_arg = float(re.split("[(]", (re.split(",",vertex)[0]))[1])
                y_arg = float(re.split("[)]", (re.split(",",vertex)[1]))[0])
                vertices.append((x_arg, y_arg))
            self.streets[name] = vertices
        except:
            print('Error: Street does not exist', file=sys.stderr)
            return

    def rm_street(self, name: str):
        try:
            del self.streets[name]
        except:
            print('Error: Street does not exist', file=sys.stderr)
            return

    def add_edge(self, vertex1_index, vertex2_index):
        self.edges.append(f"<{vertex1_index},{vertex2_index}>")
        return

    def add_vertex(self, vertex):
        index = len(self.vertices)
        self.vertices[index] = vertex

    def update_vertices(self):
        self.vertices = {}
        self.intersections = {}
        self.edges = []
        self.segments = {}
        keys = list(self.streets.keys())
        while len(keys) > 1:
            street1 = keys[0]
            keys.remove(street1)
            for street2 in keys:
                if street1 != street2:
                    for i in range(len(self.streets[street1])):
                        for j in range(len(self.streets[street2])):
                            if i != len(self.streets[street1])-1 \
                                and j != len(self.streets[street2])-1:
                                end1_line1 = self.streets[street1][i]
                                end2_line1 = self.streets[street1][i+1]
                                line1 = create_line(end1_line1, end2_line1)
                                end1_line2 = self.streets[street2][j]
                                end2_line2 = self.streets[street2][j+1]
                                line2 = create_line(end1_line2, end2_line2)
                                intersection_point = intersection(line1, line2)
                                if intersection_point and check_intersection(
                                    intersection_point, end1_line1, end2_line1
                                    , end1_line2, end2_line2):
                                    self.vertices[len(self.vertices)+1] = end1_line1
                                    self.vertices[len(self.vertices)+1] = end2_line1
                                    self.vertices[len(self.vertices)+1] = end1_line2
                                    self.vertices[len(self.vertices)+1] = end2_line2
                                    self.vertices[len(self.vertices)+1] = intersection_point
                                    vertices_indices = list(self.vertices.keys())
                                    self.intersections[f'intn{len(self.intersections)}v{int((vertices_indices)[-1])}'] = \
                                        [int((vertices_indices)[-5]), int((vertices_indices)[-4])\
                                         , int((vertices_indices)[-3]), int((vertices_indices)[-2])]
        
        self.organize_intersections()       # Organize vertices (inside organize_intersections)
                                            # and intersections
        self.get_edges()


    def organize_vertices(self):
        intermediate = {}
        for vertex in self.vertices:
            if self.vertices[vertex] not in intermediate.values():    
                intermediate[len(intermediate)+1] = self.vertices[vertex]
        return intermediate

    def organize_intersections(self):
        intermediate = self.organize_vertices()
        for vertex in self.vertices:
            repeated_vertex = self.vertices[vertex]
            for vertex2 in intermediate:
                if intermediate[vertex2] == repeated_vertex:
                    key_vertex2 = vertex2
                    for intersection in self.intersections:
                        if vertex in self.intersections[intersection]:
                            vertex_index = self.intersections[intersection].\
                                index(vertex)
                            self.intersections[intersection][vertex_index] = key_vertex2

        intersection_keys = list(self.intersections.keys())
        for intersection_key in intersection_keys:
            value_in_vertices = self.vertices[int(re.split("[v]", intersection_key)[1])]
            for key_intermediate, value_intermediate in intermediate.items():
                if value_in_vertices == value_intermediate and key_intermediate !=\
                      int(re.split("[v]", intersection_key)[1]):
                        value_to_change = self.intersections[intersection_key]
                        self.intersections[f'intn{len(self.intersections)}v{key_intermediate}']\
                              = value_to_change
                        self.intersections[intersection_key] = None
        intersection_keys = list(self.intersections.keys())
        for intersection_key in intersection_keys:
            if self.intersections[intersection_key] == None:
                del self.intersections[intersection_key]
        self.vertices = intermediate

    def segmentation(self):
        """Segment the lines into smaller lines based on intersections"""
        """The result is a dictionary with keys as the line segments and values as the vertices"""
        for intersection in self.intersections:
                end1_line1 = self.intersections[intersection][0]
                end2_line1 = self.intersections[intersection][1]
                end1_line2 = self.intersections[intersection][2]
                end2_line2 = self.intersections[intersection][3]
                if (end1_line1, end2_line1) not in self.segments:
                    self.segments[(end1_line1, end2_line1)] = [int(re.split("[v]", intersection)[1])]
                if (end1_line1, end2_line1) in self.segments and \
                    int(re.split("[v]", intersection)[1]) not in self.segments[(end1_line1, end2_line1)]:
                    self.segments[(end1_line1, end2_line1)].append(int(re.split("[v]", intersection)[1]))
                if (end1_line2, end2_line2) not in self.segments:
                    self.segments[(end1_line2, end2_line2)] = [int(re.split("[v]", intersection)[1])]
                if (end1_line2, end2_line2) in self.segments and \
                    int(re.split("[v]", intersection)[1]) not in self.segments[(end1_line2, end2_line2)]:
                    self.segments[(end1_line2, end2_line2)].append(int(re.split("[v]", intersection)[1]))

    def get_edges(self):
        self.segmentation()
        if len(self.segments) < 1:
            return []
        else:
            for segment in self.segments:
                if len(self.segments[segment]) == 1:
                    self.add_edge(segment[0], self.segments[segment][0])
                    self.add_edge(segment[1], self.segments[segment][0])
                else:
                    points_distances = {}
                    for vertex in self.segments[segment]:
                        points_distances[vertex] = (self.vertices[vertex][0] - self.vertices[segment[0]][0])**2 + (self.vertices[vertex][1]\
                            - self.vertices[segment[0]][1])**2
                        points_distances[vertex] = points_distances[vertex]**(1/2)
                    points_distances = dict(sorted(points_distances.items(), key=lambda item: item[1]), reverse=True)
                    self.add_edge(segment[0], list(points_distances.keys())[0])
                    for i in range(len(self.segments[segment])-1):
                        self.add_edge(list(points_distances.keys())[i], list(points_distances.keys())[i+1])
                    self.add_edge(segment[1], list(points_distances.keys())[-2])            

        
    def print_vertices(self):
        print("V = {")
        for vertex in self.vertices:
            formatted_value = "{:.2f}".format(self.vertices[vertex][0]) + "," + "{:.2f}".format(self.vertices[vertex][1])
            print(f"  {vertex}: ({formatted_value})")
        print("}")
    
    def print_edges(self):
        print("E = {")
        n = len(self.edges)
        for i, edge in enumerate(self.edges):
            if i < n-1:
                print(f"  {edge},")
            else:
                print(f"  {edge}")
        print("}")