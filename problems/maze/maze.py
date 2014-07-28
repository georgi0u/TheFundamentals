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
        is_out_of_bound = lambda x: x < 0 or x >= self._width
        try:
            return (
                not is_out_of_bound(coordinate[0]) and
                not is_out_of_bound(coordinate[1])
            )
        except TypeError:
            pass
        return not is_out_of_bound(coordinate)

    def _surrounding_cells(self, coordinate_pair):
        """
        Returns the Cartesian, in bound, neighbors of the maze cell described by
        parameters `x` and `y`.
        """
        x, y = coordinate_pair
        deltas = [(0, 1), (0, -1), (1, 0), (-1, 0), ]
        coordinates = [(x + delta[0], y + delta[1]) for delta in deltas]
        if x == 0 or x == self._width - 1 or y == 0 or y == self._width - 1:
            coordinates = [x for x in coordinates if self._is_in_bound(x)]
        return coordinates

    def random_cell(self):
        """
        Returns a random cell from the maze.
        """
        rand_coord = lambda: random.randint(0, self._width - 1)
        return(rand_coord(), rand_coord())

    def _prim_init(self):
        """
        Initializes the maze using Prim's algorithm. Expects the maze and
        underlying graph to be completely disconnected when called.
        """
        frontier = dict()
        frontier_list = list()

        def expand_frontier(node):
            """
            Adds the cells surrounding `node` and not already in the maze to the
            frontier.
            """
            for surrounding_cell in self._surrounding_cells(node):
                if(not self._graph.neighbors(surrounding_cell)
                   and surrounding_cell not in frontier):
                    frontier_list.append(surrounding_cell)
                    frontier[surrounding_cell] = None

        def choose_frontier_cell():
            """
            Returns and removes a random node from the frontier.
            """
            end_index = len(frontier_list) - 1
            random_index = int(random.random() * (len(frontier_list) - 1))
            frontier_choice = frontier_list[random_index]

            del frontier[frontier_choice]

            (
                frontier_list[random_index],
                frontier_list[end_index]
            ) = (
                frontier_list[end_index],
                frontier_list[random_index]
            )

            del frontier_list[end_index]

            return frontier_choice

        def add_to_maze(frontier_choice):
            """
            Adds the `frontier_choice` node to the maze by connecting it to one
            of a neighbors that's already in the maze.
            """
            surrounding_cells = [
                x for x in self._surrounding_cells(frontier_choice)
                if self._graph.neighbors(x)
            ]
            neighbor = random.choice(surrounding_cells)
            self._graph.add_edge(frontier_choice, neighbor)

        # Start by adding a random node and a random one of its neighbors to the
        # maze. Also, add all surround nodes to the frontier, i.e. a list of
        # unexplored nodes.
        start_node = self.random_cell()
        expand_frontier(start_node)
        frontier_choice = choose_frontier_cell()
        self._graph.add_edge(start_node, frontier_choice)
        expand_frontier(frontier_choice)

        # Continue exploring the frontier and adding to the maze until we run
        # out of unexplored cells
        while frontier:
            frontier_choice = choose_frontier_cell()
            add_to_maze(frontier_choice)
            expand_frontier(frontier_choice)

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
    print Maze(int(sys.argv[1]))

    return os.EX_OK

if __name__ == '__main__':
    sys.exit(main())
