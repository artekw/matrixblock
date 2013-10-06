#!/usr/bin/python2
# -*- coding: utf-8 -*-

#define DATA 5 // P9_12
#define WR 2 // P9_23
#define CS 3 // P9_15
#define CLK 4 // P9_16

from HT1632c import HT1632c

matrix = HT1632c("P8_9", "P8_7", "P8_8", "P8_10", a=1, debug=True)

#matrix.setup()

matrix.plot(1,1, 1)