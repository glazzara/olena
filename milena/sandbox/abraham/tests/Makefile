CXXFLAGS=-Wall -W -I ../../.. -g -I ../

tikz: tikz.o
	g++ ${CXXFLAGS} tikz.o -o tikz

all:

tikz.o : tikz.cc
tikz.cc: ../io/tikz/save.hh