CXX=g++-4.3
CXXOPTS=-g3 -DNDEBUG -I.

all: main

main: main.cc graph.hh get_lines.hh
	$(CXX) $(CXXOPTS) $(INCLUDES) -ltiff main.cc -o main
