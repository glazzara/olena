CXXFLAGS=-Wall -W -I ../../.. -g

test: test.o
	g++ ${CXXFLAGS} test.o -o test

test_component_tree: test_component_tree.o
	g++ ${CXXFLAGS} test_component_tree.o -o test_component_tree

test_watershed: test_watershed.o
	g++ ${CXXFLAGS} test_watershed.o -o test_watershed

all: tree

test_watershed.o: basic_najman.hh
test_component_tree.o: basic_najman.hh
test.o: basic_najman.hh