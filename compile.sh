#!/bin/sh
swig -c++ -v -python HT1632c.i
g++ -c -fPIC -c HT1632c.cpp HT1632c_wrap.cxx -I/usr/include/python2.7
g++ -shared -o _HT1632c.so HT1632c_wrap.o HT1632c.o
