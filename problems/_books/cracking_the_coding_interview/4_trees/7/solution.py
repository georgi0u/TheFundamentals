#!/usr/bin/env python2.7

import os
import sys

"""
A toy module for playing with tree algorithms.
"""

class Node(object):
    """
    A quick-and-dirty node class for verifying some tree algorithms.
    """
    def __init__(self, value):
        self._left = None
        self._right = None
        self._value = value

    @property
    def left(self):
        return self._left

    @left.setter
    def left(self, left):
        self._left = left

    @property
    def right(self):
        return self._right

    @right.setter
    def right(self, right):
        self._right = right

    @property
    def value(self):
        return self._value

    def __eq__(self, other):
        return self._value == other.value

    def __str__(self):
        return str(self._value)

def first_common_ancestor(root, node_1, node_2):
    """
    Finds the first common ancestor of `node_1 and `node_2` in the tree defined
    by the `root` parameter. Returns `None` if it can't find one, or the 
    ancestor node if it can.
    """

    def exists(root, value):
        """
        Returns -1 if there's a node with a value of `value` to the left of
        `root`, 1 if there's such a node to the right of `root`, or 0 if it 
        can't find a node on either side. Note, the root's value doesn't get 
        checked.
        """
        if not root:
            return 0

        if root.left:
            if root.left.value == value or exists(root.left, value):
                return -1
        if root.right:
            if root.right.value == value or exists(root.right, value):
                return 1
        return 0

    left_node_direction = exists(root, node_1)
    right_node_direction = exists(root, node_2)

    if left_node_direction == 0 and root.value != node_1:
        return None

    if right_node_direction == 0 and root.value != node_2:
        return None

    if left_node_direction + right_node_direction == 0:
        return root

    if left_node_direction == -1:
        if root.value == node_2:
            return root
        return first_common_ancestor(root.left, node_1, node_2)

    if root.value == node_1:
        return root
    return first_common_ancestor(root.right, node_1, node_2)

def first_common_ancestor_fast(root, node_1, node_2):
    """
    A faster implementation of the first_common_ancestor algorithm that doesn't
    traverse the entire tree with every step toward the ancestor.

    Instead, this recurses down the tree, finds the nodes in question, and then
    bubbles up the information. If a gets to the point where it's found a
    parametered node on either side, then that node is the first common 
    ancestor, and we begin to propagate that node up the recursive stack. 

    Because of that, anytime this function returns something that's not, null, 
    node1, or node2, you know that's the first_common_ancestor and can return 
    that.
    """
    if not root:
        return None

    if root.value == node_1 and root.value == node_2:
        return root

    x = first_common_ancestor_fast(root.left, node_1, node_2)
    if x and x.value not in [node_1, node_2]:
        return x

    y = first_common_ancestor_fast(root.right, node_1, node_2)
    if y and y.value not in [node_1, node_2]:
        return y

    if x and y:
        return root

    if root.value in [node_1, node_2]:
        return root

    return x if x else y

def main():
    """
    Tests various tree algorithms implemented in the above functions.
    """

    # Poor-man's initialization of a tree. Tree looks like:
    #           1
    #         2   3
    #       4   5
    #          7 6
    a                  = Node(1)
    a.right            = Node(3)

    a.left             = Node(2)
    a.left.left        = Node(4)

    a.left.right       = Node(5)
    a.left.right.right = Node(6)
    a.left.right.left  = Node(7)

    # Test things work...
    node = first_common_ancestor_fast(a, 7, 6)
    assert node and node.value == 5

    node = first_common_ancestor_fast(a, 7, 3)
    assert node and node.value == 1

    node = first_common_ancestor_fast(a, 6, 4)
    assert node and node.value == 2

    node = first_common_ancestor_fast(a, 4, 6)
    assert node and node.value == 2

    node = first_common_ancestor_fast(a, 2, 4)
    assert node and node.value == 2

    node = first_common_ancestor_fast(a, 1, 1)
    assert node and node.value == 1

    node = first_common_ancestor_fast(None, 9, 1)
    assert not node

    node = first_common_ancestor_fast(a, 9, 1)
    assert not node
 
    node = first_common_ancestor_fast(Node(1), 9, 1)
    assert not node
 
    node = first_common_ancestor_fast(Node(1), 1, 1)
    assert not node
 
    node = first_common_ancestor_fast(Node(1), 3, 2)
    assert not node

    print "Asserts Passed"

    return os.EX_OK

if __name__ == '__main__':
    sys.exit(main())
