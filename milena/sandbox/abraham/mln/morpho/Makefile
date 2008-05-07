SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
EXEC=$(SRC:.cc=)
CC=g++
CXXFLAGS=-Wall -W -Werror -I ../../../.. -g

all: $(EXEC)

clean:
	rm -rf *.o
	rm -rf *~

distclean: clean
	rm -rf $(EXEC)

.PHONY: clean distclean

## Depedencies
test_watershed.o: basic_najman.hh
test_watershed_topo.o: basic_najman.hh
test_component_tree.o: basic_najman.hh
test.o: basic_najman.hh