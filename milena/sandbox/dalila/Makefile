OLENA_INCLUDE_PATH=../../../
CXXFLAGS=-I$(OLENA_INCLUDE_PATH) -ggdb3 -Wall -O1 -DNDEBUG

all: photo table

photo: demat.hh photo.cc
	g++ $(CXXFLAGS) photo.cc -o photo

table: demat.hh table.cc
	g++ $(CXXFLAGS) table.cc -o table

dclean: clean
	rm -f photo
	rm -f table

clean:
	rm -f *.pbm *.pgm *.ppm

