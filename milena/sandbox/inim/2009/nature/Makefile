SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
EXEC=$(SRC:.cc=)
CC=g++
CXXFLAGS=-Wall -W -I ../abraham -I ../.. -O3 -DNDEBUG

fft: fft.o
	$(CC) $(CXXFLAGS) $< -o $@ -l fftw3

nature: nature.o
	$(CC) $(CXXFLAGS) $< -o $@ -l fftw3

all: $(EXEC)

clean:
	rm -rf *.o
	rm -rf *~

distclean: clean
	rm -rf $(EXEC)

proj: proj.cc proj.hh

.PHONY: clean distclean
