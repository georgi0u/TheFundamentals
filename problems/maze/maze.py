#!/usr/bin/env python2.7

import random

class Graph(object):
    def __init__(self):
        self._adjacency_list = dict()

    def adjacent(self, node_1, node_2):
        if(node_1 not in self._adjacency_list or
           node_2 not in self._adjacency_list):
            raise Exception('Node doesn\'t exist')

        return node_1 in self._adjacency_list[node_2]

    def neighbors(self, node_1):
        if node_1 not in self._adjacency_list:
            raise Exception('Node doesn\'t exist')

        return  self._adjacency_list[node_1]

    def add_node(self, node):
        if node in self._adjacency_list:
            raise Exception('Node already exists')

        self._adjacency_list[node] = []

    def add_edge(self, node_1, node_2):
        def add_directed_edge(node_1, node_2):
            if node_1 not in self._adjacency_list:
                raise Exception('Node doesn\'t exist')
            self._adjacency_list[node_1].append(node_2)

        add_directed_edge(node_1, node_2)
        add_directed_edge(node_2, node_1)

    def remove_edge(self, node_1, node_2):
        if(node_1 not in self._adjacency_list or
           node_2 not in self._adjacency_list):
            raise Exception('Node doesn\'t exist') 

        self._adjacency_list[node_1].remove(node_2)
        self._adjacency_list[node_2].remove(node_1)

class Maze(Graph):
    def __init__(self, width):
        Graph.__init__(self)

        self._width = width

        for row in range(0, width):
            for col in range(0, width):
                self.add_node((row, col))

        self.prim_init()

    def _is_in_bound(self, coordinate):
        return coordinate >= 0 and coordinate < self._width

    def surrounding_cells(self, x, y):
        def get_deltas(coordinate):
            delta_coordinates = []
            deltas = [-1, 1]
            for delta in deltas:
                neighbor = coordinate + delta
                if self._is_in_bound(neighbor):
                    delta_coordinates.append(neighbor)
            return delta_coordinates

        ret = []
        ret.extend([(dx, y) for dx in get_deltas(x)])
        ret.extend([(x, dy) for dy in get_deltas(y)])
        return ret

    def prim_init(self):

        frontier = [(0,0)]
        while len(frontier) != 0:
            frontier_cell = random.choice(frontier)
            frontier.remove(frontier_cell)

            neighbor_cell = random.choice(
                self.surrounding_cells(*frontier_cell)
            )

            self.add_edge(frontier_cell, neighbor_cell)

            for surrounding_cell in self.surrounding_cells(*neighbor_cell):
                if len(self.neighbors(surrounding_cell)) == 0:
                    frontier.append(surrounding_cell)

            print self.__str__()



    def __unicode__(self):
        string = u' '
        string += '_'*(self._width*2-1)
        string += '\n'
        for row in range(0, self._width):
            string += '|'
            for col in range(0, self._width):
                neighbors = self._adjacency_list[(row, col)]
                string += '_' if (col, row+1) not in neighbors else ' '
                string += '|' if (col+1, row) not in neighbors else ' '

            string += '\n'
        string += '\n'
        return string

    def __str__(self):
        return unicode(self).encode('utf-8')


    # def __init__(self, width):
    #     self._adjacency_matrix = []
    #     for i in range(0, width**2):
    #         self._adjacency_matrix.append([0] * width**2)

    #     current_point = (0, 0)
    #     self._adjacency_matrix[0][0] = 1

    #     wall_list = []
    #     for neighbor in self.neighbors(*current_point):
    #         if not self.is_free(*neighbor):
    #             wall_list.append(neighbor)

    #     while len(wall_list) != 0:

    #         wall = random.choice(wall_list)
    #         wall_list.remove(wall)

    #         new_current = None
    #         if wall[0] > current_point[0] and self._is_in_bounds(wall[0] + 1):
    #             new_current = (wall[0] + 1, wall[1])
    #         elif wall[0] < current_point[0] and self._is_in_bounds(wall[0] - 1):
    #             new_current = (wall[0] - 1, wall[1])
    #         elif wall[1] > current_point[1] and self._is_in_bounds(wall[1] + 1):
    #             new_current = (wall[0], wall[1] + 1)
    #         elif wall[1] < current_point[1] and self._is_in_bounds(wall[1] - 1):
    #             new_current = (wall[0], wall[1] - 1)



    #         if not new_current:
    #             continue

    #         if self.is_free(*new_current):
    #             continue

    #         self.open(*wall)
    #         self.open(*new_current)

    #         neighbor_filter = lambda coordinate: self.is_wall(*coordinate)
    #         wall_list.extend(
    #             self.neighbors(
    #                 *new_current, neighbor_filter = neighbor_filter
    #             )
    #         )

    #         print self.__str__()
    #         raw_input()




    # def neighbors(self, x, y, neighbor_filter=None):
    #     def get_deltas(coordinate):
    #         delta_coordinates = []
    #         deltas = [-1, 1]
    #         for delta in deltas:
    #             neighbor = coordinate + delta
    #             try:
    #                 self._check_bound(neighbor)
    #                 delta_coordinates.append(neighbor)
    #             except IndexError as error:
    #                 pass
    #         return delta_coordinates

    #     ret = []
    #     ret.extend([(dx, y) for dx in get_deltas(x)])
    #     ret.extend([(x, dy) for dy in get_deltas(y)])
    #     if neighbor_filter:
    #         ret = filter(neighbor_filter, ret)

    #     return ret

    # def _is_in_bounds(self, x_or_y):
    #     return x_or_y >= 0 and x_or_y < len(self._adjacency_matrix)

    # def _check_bound(self, x_or_y):
    #     if not self._is_in_bounds(x_or_y):
    #         raise IndexError('Coordinate out of bounds')

    # def _check_bounds(self, x, y):
    #     [self._check_bound(coordinate) for coordinate in [x, y]]

    # def is_free(self, x, y):
    #     self._check_bounds(x, y)
    #     return self._adjacency_matrix[x][y] == 1

    # def is_wall(self, x, y):
    #     self._check_bounds(x, y)
    #     return self._adjacency_matrix[x][y] == 0

    # def block(self, x, y):
    #     self._check_bounds(x, y)
    #     self._adjacency_matrix[x][y] = 0

    # def open(self, x, y):
    #     self._check_bounds(x, y)
    #     self._adjacency_matrix[x][y] = 1

    # def __unicode__(self):
    #     string = u''
    #     for row in self._adjacency_matrix:
    #         for cell in row:
    #             if cell == 1:
    #                 string += ' '
    #             else:
    #                 string += u'\u2588'
    #         string += '\n'
    #     return string.rstrip()

    # def __str__(self):
    #     return unicode(self).encode('utf-8')


maze = Maze(10)
print maze
