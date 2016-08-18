#!/usr/bin/env python

#import numpy as np


class LinearRegression:
    def __init__(self):
        self.Sx = 0.
        self.Sy = 0.
        self.Sxx = 0.
        self.Sxy = 0.
        self.Syy = 0.
        self.n = 0

    def add(self, x, y):
        self.Sx += x
        self.Sy += y
        self.Sxx += x * x
        self.Sxy += x * y
        self.Syy += y * y
        self.n += 1

    def add_n(self, x, y):
        for xx, yy in zip(x, y):
            self.add(xx, yy)

    def alpha(self):
        if self.n < 1:
            return float('NaN')
        _alpha = (self.Sy * self.Sxx - self.Sx * self.Sxy) / (self.n * self.Sxx - self.Sx * self.Sx)
        return _alpha

    def beta(self):
        if self.n < 1:
            return float('NaN')
        _beta = (self.n * self.Sxy - self.Sx * self.Sy) / (self.n * self.Sxx - self.Sx * self.Sx)
        return _beta


# ______________________________________________________________________________
if __name__ == '__main__':

    a, b = 35, 15
    x = [0, 2, 4, 6]
    y = [a + b*xx for xx in x]

    reg = LinearRegression()
    reg.add_n(x, y)

    print "Expect: alpha=%f, beta=%f" % (a, b)
    print "Got   : alpha=%f, beta=%f" % (reg.alpha(), reg.beta())

