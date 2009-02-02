SRC=test_trans_image.cc
OBJ=${SRC:.cc=.o}
CXXFLAGS=-I ~/lrde/olena/trunk/milena

all: $(OBJ)
	g++ $<

%.o: %cc
	g++ $(CXXFLAGS) -o$@ $<

clean:
	rm -f *~ *.o a.out