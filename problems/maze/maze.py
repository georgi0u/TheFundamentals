#!/usr/bin/env python2.7

import random

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

        return  self._adjacency_list[node]

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
            if node_1 not in self._adjacency_list:
                raise Exception('Node doesn\'t exist')
            self._adjacency_list[node_1].append(node_2)

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
    def __init__(self, width):
        self._graph = Graph()

        self._width = width
        self._cells_in_maze = []

        for row in range(0, width):
            for col in range(0, width):
                node = (row, col)
                self._graph.add_node(node)

        self.prim_init()

    def _is_in_bound(self, coordinate):
        return coordinate >= 0 and coordinate < self._width

    def surrounding_cells(self, x, y):
        def get_deltas(coordinate):
            return filter(self._is_in_bound, [coordinate + d for d in [-1, 1]])

        ret = []
        ret.extend([(dx, y) for dx in get_deltas(x)])
        ret.extend([(x, dy) for dy in get_deltas(y)])
        return ret

    def test_init(self):
        for i in range(0, self._width-1):
            node_1 = (i,0)
            node_2 = (i+1,0)
            self._graph.add_edge((i, 0), (i+1, 0))

    def prim_init(self):
        start_node = (
            random.randint(0,self._width-1), 
            random.randint(0,self._width-1)
        )
        self._cells_in_maze.append(start_node)
        frontier = self.surrounding_cells(*start_node)

        while frontier: 
            frontier_choice = random.choice(frontier)
            frontier.remove(frontier_choice)

            surrounding_cells = filter(
                lambda x: x in self._cells_in_maze,
                self.surrounding_cells(*frontier_choice)
            )

            neighbor = random.choice(surrounding_cells)

            self._graph.add_edge(frontier_choice, neighbor)
            self._cells_in_maze.append(frontier_choice)

            new_frontier = filter(
                lambda x: x not in self._cells_in_maze,
                self.surrounding_cells(*frontier_choice)
            )

            new_frontier = filter(
                lambda x: x not in frontier,
                new_frontier)

            frontier.extend(new_frontier)

    def __unicode__(self):
        string = u' '
        string += '_'*(self._width*2-1)
        string += '\n'
        for col in range(0, self._width):
            string += '|'
            for row in range(0, self._width):
                neighbors = self._graph.neighbors((row, col))
                string += '_' if (row, col+1) not in neighbors else ' '
                string += '|' if (row+1, col) not in neighbors else ' '
            string += '\n'
        string += '\n'

        return string

    def __str__(self):
        return unicode(self).encode('utf-8')

maze = Maze(50)

print maze
