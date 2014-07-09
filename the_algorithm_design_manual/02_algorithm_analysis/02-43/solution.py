#!/usr/bin/env python2.7

"""
An attempt at an answer for question 02-43 in The Algorithm Design Manual.
See the README for more details.
"""

# Future
from __future__ import print_function

# Std
import random
import os
import sys


def build_s_prime(set_1, size_of_new_set):
    """
    Builds a new set using the members of `set_1` such that every member in the
    `set_1` has an equal probability of being in the new set.
    """

    print(set_1, size_of_new_set)

    s_prime = set()

    single_element_probability = size_of_new_set/float(len(set_1))

    for element in set_1:
        random_number = random.random()
        print(single_element_probability, random_number)
        if single_element_probability <= random_number:
            s_prime.update([element])

    return s_prime



def main():
    size_of_set_1 = random.randint(1, 100)
    set_1 = range(size_of_set_1)

    size_of_set_1_prime = random.randint(1, size_of_set_1)
    set_1_prime = build_s_prime(set_1, size_of_set_1_prime)

    print(size_of_set_1_prime, set_1_prime)
    return os.EX_OK

if __name__ == '__main__':
    sys.exit(main())
