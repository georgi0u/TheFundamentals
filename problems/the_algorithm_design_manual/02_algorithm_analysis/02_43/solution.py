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
import pprint


def random_sample(source, sample_size):
    """
    Builds a new set using the members of `source` such that every member in
    the `source` set has an equal probability of being in the random sample.
    """

    if sample_size > len(source):
        raise Exception(
            'Can\'t create a random sample bigger than the source set'
        )

    random_sample_list = list()
    already_chosen_index = len(source)

    while len(random_sample_list) < sample_size:
        # Pick a random element from the source
        to_include_index = int(random.random() * len(source))

        # if the element is in the portion of the source we've 
        # designated for already-encountered elements, try again.
        if to_include_index >= already_chosen_index:
            continue


        # Move the pointer (think stack pointer) one back, so that we can fit 
        # one more element in the already-encountered portion of the source,
        # then swap the chosen element with the one at that index.
        already_chosen_index -= 1
        (
            source[to_include_index],
            source[already_chosen_index]
        ) = (
            source[already_chosen_index],
            source[to_include_index]
        )

        # Add the chosen element to the sample
        random_sample_list.append(source[already_chosen_index])

    return random_sample_list

def random_sample_2(source, sample_size):
    """
    Builds a new set using the members of `source` such that every member in
    the `source` set has an equal probability of being in the random sample.

    Does so without factoring in the length of source.
    """

    random_sample_list = list()

    for i in range(sample_size):
        random_sample_list.append(source[i])

    # For every index in the remainder of the source set not yet in the random 
    # sample...
    for i in range(sample_size, len(source)):
        # Get a random int between 0 and that random index in the source set
        j = random.randint(0, i) 

        # If that random integer is a valid index in our random_sample, set the 
        # sample index's value to that of the source index's value.
        if j < sample_size:
            random_sample_list[j] = source[i]

    return random_sample_list

def main():
    """
    Prints random samples of size 10 of a set of integers [0,99]
    """

    source_set_size = 100
    source_set = range(source_set_size)

    random_sample_size = 10

    random_sample_set = random_sample_2(source_set, random_sample_size)
    pprint.pprint(random_sample_set)

    return os.EX_OK

if __name__ == '__main__':
    sys.exit(main())
