#!/usr/bin/env python2.7

"""
A module containing basic graph classes and traversal functions.
"""

from Queue import Queue
import sys
import random
import pprint

class Graph(object):
    """
    A very silly adjacency list implementation of a graph.
    """

    def __init__(self):
        """
        Initializes the adjacency list
        """
        self._adjacency_list = list()

    def exists(self, node):
        """
        Returns `True` if node exists in the graph, false otherwise.
        """
        return node >= 0 and node < len(self._adjacency_list)

    def get_nodes(self):
        """
        Returns a list of the nodes in the graph.
        """
        return range(0, len(self._adjacency_list))

    def neighbors(self, node):
        """
        Returns a list of all the nodes which neighbor the parameter node.
        """
        if not self.exists(node):
            raise IndexError('Node doesn\'t exist')

        return self._adjacency_list[node]

    def add_node(self):
        """
        Adds an edgeless node to the graph. Returns its id.
        """
        self._adjacency_list.append(set())
        return len(self._adjacency_list)-1

    def add_edge(self, node_1, node_2):
        """
        Adds an edge between parameters `node_1` and `node_2`.
        """
        if not self.exists(node_1) or not self.exists(node_2):
            raise IndexError('Node isn\'t in the graph')

        if node_1 == node_2:
            return

        self._adjacency_list[node_1].add(node_2)
        self._adjacency_list[node_2].add(node_1)

    def __str__(self):
        string = ''
        for node, neighbors in enumerate(self._adjacency_list):
            string += '{}: '.format(node)
            for node in neighbors:
                string += '{} '.format(node)
            string += '\n'

        return string


def breadth_first_search(graph, start, end):
    neighbors = Queue()
    neighbors.put(start)

    visited = {}
    visited[start] = None

    while not neighbors.empty():
        current = neighbors.get()

        if current == end:
            break

        for node in graph.neighbors(current):
            if node not in visited:
                neighbors.put(node)
                visited[node] = current

    current = end
    path = [current]
    while current != start:
       current = visited[current]
       path.append(current)

    return path

def main():
    """
    Playing with graphs...
    """
    graph = Graph()

    for i in range(1000):
        graph.add_node()

    for i in graph.get_nodes():
        graph.add_edge(i, random.randint(0, i))
        graph.add_edge(i, random.randint(0, i))
        graph.add_edge(i, random.randint(0, i))
        graph.add_edge(i, random.randint(0, i))

    print graph
    path = breadth_first_search(graph,0,999)
    print path
    for node in path:
        print "{}: ".format(node),
        for node in graph.neighbors(node):
            print node,
        print

if __name__ == '__main__':
    sys.exit(main())
