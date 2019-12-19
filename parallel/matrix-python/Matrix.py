from random import random
import threading

class Matrix:

    def __init__(self, m, n):
        self.data = []
        self.m = m
        self.n = n
        for i in range(m):
            row = []
            for j in range(n):
                row.append(0.0)
            self.data.append(row)

    def fillRandom(self):
        for i in range(self.m):
            rr = random()*self.m
            for j in range(self.n):
                self.data[i][j] = random()+rr

    def add(self, other):
        for i in range(self.m):
            for j in range(self.n):
                self.data[i][j] += other.data[i][j]

    def multiply(self, other):
        mult = Matrix(self.m, other.n)
        for i in range(mult.m):
            for j in range(mult.n):
                s = 0.0
                for k in range(self.n):
                    s += self.data[i][k] * other.data[k][j]
                mult.data[i][j] = s
        return mult

    def sort(self):
        self.data.sort(key = sum)

    def __repr__(self):
        r = ""
        for i in range(self.m):
            for j in range(self.n):
                r += "{:5.2f}".format(self.data[i][j]) + "\t"
            r += "\n"
        return r


