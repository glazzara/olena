SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
EXEC=$(SRC:.cc=)
CC=g++
CXXFLAGS=-Wall -W -Werror -I ../../ -I ../../../../ -g

all: $(EXEC)

clean:
	rm -rf *.o
	rm -rf *~

distclean: clean
	rm -rf $(EXEC)

.PHONY: clean distclean

## Depedencies
tikz.o: ../../../mln/io/tikz/save.hh