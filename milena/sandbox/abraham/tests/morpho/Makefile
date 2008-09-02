SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
EXEC=$(SRC:.cc=)
CC=g++
CXXFLAGS=-Wall -W -I ../../ -I ../../../../ -g

all: $(EXEC)

clean:
	rm -rf *.o
	rm -rf *~

distclean: clean
	rm -rf $(EXEC)

.PHONY: clean distclean

## Depedencies
test_watershed_topo.cc: ../../mln/morpho/basic_najman.hh
test_watershed.cc: ../../mln/morpho/basic_najman.hh
test_componenet_tree.cc: ../../mln/morpho/basic_najman.hh