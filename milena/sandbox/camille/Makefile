CXXFLAGS = -ggdb -DNDEBUG -O2 -I../..

all: graph-line-gradient-closing graph-wst

test-graph-wst: graph-wst
	./graph-wst ../../img/small.pgm 20 out.ppm

test-graph-line-gradient-closing: graph-line-gradient-closing
	./graph-line-gradient-closing ../../img/small.pgm 20 out.pgm
