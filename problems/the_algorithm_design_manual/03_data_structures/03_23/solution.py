#!/usr/bin/env python2.7

class LinkedList(object):
    class Node(object):
        def __init__(self, value):
            self._next = None
            self._value = value

        def set_next(self, node):
            self._next = node

        def get_next(self):
            return self._next

        def value(self):
            return self._value

    def __init__(self):
        self._head = None
        self._tail = None

    def get_head(self):
        return self._head

    def append(self, value):
        node = LinkedList.Node(value)
        if self._head == None:
            self._head = node

        if self._tail != None:
            self._tail.set_next(node)

        self._tail = node

    def __str__(self):
        next = self._head
        string = ''
        while(next != None):
            string += '{} -> '.format(next.value())
            next = next.get_next()
        return string

    def reverse(self):

        def reverse_impl(node):
            if(node.get_next() == None):
                return (node, node)

            head, tail = reverse_impl(node.get_next())
            tail.set_next(node)
            node.set_next(None)
            return head, node

        self._head, self._tail = reverse_impl(self._head)

    def reverse_non_recursive(self):
        

l = LinkedList()
l.append(1)
l.append(2)
l.append(3)
l.append(4)
l.append(5)

print l

l.reverse()

print l
