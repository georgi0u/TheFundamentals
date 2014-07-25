#!/usr/bin/env python2.7

class Maze(object):

    def __init__(self, width):
        self._adjacency_matrix = []
        for i in range(0,width):
            self._adjacency_matrix.append([0]*width)

        current_point = (0,0)
        self._adjacency_matrix[0][0] = 1

        wall_list = []
        for neighbor in self.neighbors(*current_point):
            if not self.is_free(*neighbor):
                wall_list.append(neighbor)

        while not len(wall_list):
            cell = random.choice(wall_list)
            if cell[0] > current_point[0]



    def neighbors(self, x, y):
        ret = []
        deltas = [-1, 1]
        for dx in deltas:
            neighbor = (x+dx, y)
            try:
                self._check_bounds(*neighbor)
                ret.append(neighbor)
            except IndexError as error:
                pass

        for dy in deltas:
            neighbor = (x, y+dy)
            try:
                self._check_bounds(*neighbor)
                ret.append(neighbor)
            except IndexError as error:
                pass

        return ret

    def _check_bounds(self, x, y):
        if x < 0 or x > len(self._adjacency_matrix):
            raise IndexError('X Coordinate out of bounds')
        if y < 0 or y > len(self._adjacency_matrix):
            raise IndexError('Y Coordinate out of bounds')

    def is_free(self, x, y):
        self._check_bounds(x,y)
        return self._adjacency_matrix[x][y] == 1

    def block(self, x, y):
        self._check_bounds(x, y)
        self._adjacency_matrix[x][y] = 0

    def open(self, x, y):
        self._check_bounds(x, y)
        self._adjacency_matrix[x][y] = 1

    def __unicode__(self):
        string = u''
        for row in self._adjacency_matrix:
            for cell in row:
                if cell == 1:
                    string += ' '
                else:
                    string += u'\u2588'
            string += '\n'
        return string.rstrip()

    def __str__(self):
        return unicode(self).encode('utf-8')


maze = Maze(10)
maze.block(1,1)
for neighbor in maze.neighbors(4,4):
    maze.open(*neighbor)
print maze
