INCLUDES=-I "../../../../"
CXX=g++
CXX_FLAGS=-W -Wall -DNDEBUG -O3 -ffast-math

OBJS= main.o
TARGET=cutthem

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(DEBUG_FLAGS) $^ -o $@

clean:
	rm -f *~ *.o

distclean: clean
	rm -f cutthem
	rm -rf *_result

%.o: %.cc
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(DEBUG_FLAGS) -c $<

main.o: cuttor.hh cuttor.hxx				\
	advance_iterator.hh advance_iterator.hxx	\
	vector.hh vector.hxx				\
	vect_it.hh vect_it.hxx				\
	vect_it_OLD.hxx

#