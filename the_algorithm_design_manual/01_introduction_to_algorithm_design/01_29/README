# 01-29

## Problem

There are 25 horses. At most, 5 horses can race together at a time. 
You must determine the fastest, second fastest, and third fastest horses. 
Find the minimum number of races in which this can be done.

## Solution

We start with 25 horses:

    1a 2a 3a 4a 5a
    1b 2b 3b 4b 5b
    1c 2c 3c 4c 5c
    1d 2d 3d 4d 5d
    1e 2e 3e 4e 5e

### Step 1

We race all the horses with the same letter against each other.
So now we know the top horses in each letter group. 
Assume the each horse in each letter group is ordered fastest to slowest,

**Race count: 5**

### Step 2

We then race each #1 horse from each letter group against each other.
Assume that race turns out such at `a` is the fastest, `b` is the second fastest, etc.

We now know that `1c` is (1) faster than `1d` and (2) faster than all the other `c` horses, and 
since `1d` is faster than all the other `d` horses we can infer via the transitive property that `1c` is 
faster than all the `d` horses and similarly it's faster than all of the `e` horses.

Our contenders are now:

    1a 2a 3a 4a 5a
    1b 2b 3b 4b 5b
    1c 2c 3c 4c 5c

We also know that the `4` and `5` horses can't be in the top three since there are at least 3 horses in front of each,
which we know are faster.

Our contenders are now:

    1a 2a 3a
    1b 2b 3b
    1c 2c 3c

Knowing `1c` was the slowest of the `1` group and yet was faster than all other contenders in its letter group, 
we can eliminate `2c` through `3c`

    1a 2a 3a
    1b 2b 3b
    1c

We can also eliminate `3b` since there are at least three horses we know to be faster, e.g., `1b`, `2b`, and `1a`.

    1a 2a 3a
    1b 2b
    1c

**Race count: 6**

At this point we know `1a` is the fastest horse, and we have 5 remaining contenders for 2nd and 3rd place.

We race those contenders, the winner takes overall second, and second takes third overall.

**Race count: 7**

