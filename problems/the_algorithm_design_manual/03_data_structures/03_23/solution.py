#!/usr/bin/env python2.7

"""
A toy module for demonstrating and playing around with some linked list
operations.
"""

import os
import sys


class LinkedList(object):

    """
    An incomplete LinkedList class implementation for playing around with some
    algorithms.
    """

    class Node(object):

        """
        A node class used to encapsulate the stored data and the connection
        between that data. This implementation is singly linked, i.e., the
        connection between nodes goes in one direction and not the other.
        """

        def __init__(self, value):
            """
            Sets the value of the node and initial link to other nodes
            (initially `None`).
            """
            self._next = None
            self._value = value

        def get_next(self):
            """
            Returns this node's neighbor, i.e., the next node -- if any -- this
            node is linked to.
            """
            return self._next

        def set_next(self, node):
            """
            Set's this node's link to another node.
            """
            self._next = node

        def __str__(self):
            """
            Returns the value of the node.
            """
            return str(self._value)

    def __init__(self, iterable_data=None):
        """
        Initializes the list with pointers to the head and tail of the list.
        (We maintain a reference to the tail of the list to make appends
        cheaper.)
        """
        self._head = None
        self._tail = None

        if not iterable_data:
            return

        for value in iterable_data:
            self.append(value)

    def append(self, value):
        """
        Adds a node, containing `value`, to the end of the list.
        """
        node = LinkedList.Node(value)
        if self._head == None:
            self._head = node

        if self._tail != None:
            self._tail.set_next(node)

        self._tail = node

    def __str__(self):
        """
        Returns a string representation of all the list's contained nodes and
        their links to one another.
        """
        the_next = self._head
        string = ''
        while the_next != None:

            string += '{} -> '.format(the_next)
            the_next = the_next.get_next()
        return string + 'None'

    def reverse_recursive(self):
        """
        Reverses the contents of the list using recursion.

        >>> l = LinkedList([1,2,3])
        >>> l.reverse_recursive()
        >>> print l
        3 -> 2 -> 1 -> None
        """
        def reverse_impl(node):
            """
            Implement ion of the recursive reverse. Allows us to mask the need
            for a parameter from the calling code.
            """
            if not node:
                return None

            if not node.get_next():
                self._head = node
                return node

            sub_tail = reverse_impl(node.get_next())
            sub_tail.set_next(node)
            node.set_next(None)
            return node

        reverse_impl(self._head)

    def reverse_iterative(self):
        """
        Reverses the contents of the list using iteration.

        >>> l = LinkedList([1,2,3])
        >>> l.reverse_iterative()
        >>> print l
        3 -> 2 -> 1 -> None
        """
        if self._head == None or self._head == None:
            return

        current = self._head
        the_next = current.get_next()
        while the_next:
            temp = the_next.get_next()
            the_next.set_next(current)
            current = the_next
            the_next = temp

        self._head.set_next(None)
        self._head = current


def main():
    """
    Sets up a few test cases and prints out some sample work.
    """
    import doctest
    doctest.testmod()

    print 'O length list'
    print '-------------'
    linked_list = LinkedList()
    print linked_list
    linked_list.reverse_recursive()
    print linked_list
    linked_list.reverse_iterative()
    print linked_list
    print

    print '1 length list'
    print '-------------'
    linked_list.append(1)
    print linked_list
    linked_list.reverse_recursive()
    print linked_list
    linked_list.reverse_iterative()
    print linked_list
    print

    print '2 length list'
    print '-------------'
    linked_list.append(2)
    print linked_list
    linked_list.reverse_recursive()
    print linked_list
    linked_list.reverse_iterative()
    print linked_list
    print

    linked_list.append(3)
    linked_list.append(4)
    linked_list.append(5)

    print '> 2 length list'
    print '--------------'
    print linked_list
    linked_list.reverse_iterative()

    print linked_list
    linked_list.reverse_recursive()

    print linked_list
    print

    return os.EX_OK

if __name__ == '__main__':
    sys.exit(main())
