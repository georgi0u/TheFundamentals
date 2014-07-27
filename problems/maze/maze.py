#!/usr/bin/env python2.7

"""
A play-thing of a module for messing around with graphs, mazes, and various
algorithms associated with each.
"""

import os
import random
import sys

class Graph(object):

    """
    A graph class implemented using adjacency lists.
    """

    def __init__(self):
        self._adjacency_list = dict()

    def adjacent(self, node_1, node_2):
        """
        Returns true if node_1 and node_2 are adjacent to one another, else
        otherwise. Expects node_1 and node_2 to be in the graph.
        """
        if(node_1 not in self._adjacency_list or
           node_2 not in self._adjacency_list):
            raise Exception('Node doesn\'t exist')

        return node_1 in self._adjacency_list[node_2]

    def neighbors(self, node):
        """
        Returns a list of the nodes adjacent to `node`. Expects `node` to be
        in the graph.
        """
        if node not in self._adjacency_list:
            raise Exception('Node doesn\'t exist')

        return self._adjacency_list[node]

    def add_node(self, node):
        """
        Adds `node` to the graph.
        """
        if node in self._adjacency_list:
            raise Exception('Node already exists')

        self._adjacency_list[node] = []

    def add_edge(self, node_1, node_2):
        """
        Adds an edge between `node_1` and `node_2`.
        """
        def add_directed_edge(node_1, node_2):
            """Implementation of the edge creation logic"""
            if node_1 not in self._adjacency_list:
                raise Exception('Node doesn\'t exist')
            self._adjacency_list[node_1].append(node_2)

        # Graphs of this type are implicitly un-directed.
        add_directed_edge(node_1, node_2)
        add_directed_edge(node_2, node_1)

    def remove_edge(self, node_1, node_2):
        """
        Removes all edges between `node_1` and `node_2`.
        """
        if(node_1 not in self._adjacency_list or
           node_2 not in self._adjacency_list):
            raise Exception('Node doesn\'t exist')

        self._adjacency_list[node_1].remove(node_2)
        self._adjacency_list[node_2].remove(node_1)


class Maze(object):

    """
    A class for generating and printing random mazes.
    """

    def __init__(self, width):
        """
        Initializes a completely disconnected graph with `width`^2 nodes.
        Then creates a maze by connected said nodes via Prim's algorithm.
        """
        self._graph = Graph()

        self._width = width
        self._cells_in_maze = []

        for row in range(0, width):
            for col in range(0, width):
                node = (row, col)
                self._graph.add_node(node)

        self._prim_init()

    def _is_in_bound(self, coordinate):
        """
        Returns true if `coordinate` -- either and x or y -- is in bounds, false
        otherwise.
        """
        return coordinate >= 0 and coordinate < self._width

    def _surrounding_cells(self, x, y):
        """
        Returns the Cartesian, in bound, neighbors of the maze cell described by
        parameters `x` and `y`.
        """
        def get_deltas(coordinate):
            """
            Get all valid -- i.e., inbound -- changes to `coordinate`.
            """
            deltas = []
            for delta in [-1, 1]:
                new_coordinate = coordinate + delta
                if self._is_in_bound(new_coordinate):
                    deltas.append(new_coordinate)
            return deltas

        ret = []
        ret.extend([(dx, y) for dx in get_deltas(x)])
        ret.extend([(x, dy) for dy in get_deltas(y)])
        return ret

    def _prim_init(self):
        """
        Initializes the maze using Prim's algorithm. Expects the maze and
        underlying graph to be completely disconnected when called.
        """
        start_node = (
            random.randint(0, self._width - 1),
            random.randint(0, self._width - 1)
        )
        self._cells_in_maze.append(start_node)
        frontier = self._surrounding_cells(*start_node)

        while frontier:
            frontier_choice = random.choice(frontier)
            frontier.remove(frontier_choice)

            surrounding_cells = [
                x for x in self._surrounding_cells(*frontier_choice)
                if x in self._cells_in_maze
            ]

            neighbor = random.choice(surrounding_cells)

            self._graph.add_edge(frontier_choice, neighbor)
            self._cells_in_maze.append(frontier_choice)

            frontier.extend([
                x for x in self._surrounding_cells(*frontier_choice)
                if x not in self._cells_in_maze and x not in frontier
            ])

    def __unicode__(self):
        """
        Prints the maze.
        """
        string = u' '
        string += '_' * (self._width * 2 - 1)
        string += '\n'
        for col in range(0, self._width):
            string += '|'
            for row in range(0, self._width):
                neighbors = self._graph.neighbors((row, col))
                string += '_' if (row, col + 1) not in neighbors else ' '
                string += '|' if (row + 1, col) not in neighbors else ' '
            string += '\n'
        string += '\n'

        return string

    def __str__(self):
        """
        Prints the maze.
        """
        return unicode(self).encode('utf-8')


def main():
    """
    Generates and prints a maze.
    """
    maze = Maze(50)
    print maze

    return os.EX_OK

if __name__ == '__main__':
    sys.exit(main())
