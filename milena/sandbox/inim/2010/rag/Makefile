SRC=rag.cc
HEADERS=center_weight.hh dijkstra.hh p_vertices_with_accu.hh rag.hh
OBJ=$(SRC:.cc=.o)
TEST=rag
CXX=g++-4.3
CXXFLAGS=-W -Wall -O2 -DNDEBUG
CPPFLAGS=-I../../../../
LD=$(CXX)
LDFLAGS=

all: $(TEST)

$(TEST): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(TEST)

.cc.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~

distclean: clean
	rm -f $(TEST)

rag.o: rag.cc $(HEADERS)
