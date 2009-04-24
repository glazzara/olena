INCLUDES=-I "../../../../"
CXX=g++-4.2
CXX_FLAGS=-W -Wall -O3 -ffast-math

OBJS= main.o
TARGET=cutthem

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

clean:
	rm -f *~ *.o

distclean: clean
	rm -f cutthem
	rm -rf *_result

%.o: %.cc
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $<

main.o: cuttor.hh cuttor.hxx vector.hh vector.hxx vect_it.hh vect_it.hxx

#