CXX=g++
CXXFLAGS=-Wall -Wextra -W -Werror -pedantic

DEBUG_FLAGS = 
OPT_FLAGS =  -DNDEBUG -O3
CXXFLAGS += ${DEBUG_FLAGS}
CXXFLAGS += ${OPT_FLAGS}

SRC = morpho.cc
HEADERS = $(SRC:.cc=.hh)
OBJS = $(SRC:.cc=.o)

LIB_PATH=
LIB=


all: morpho

clean:
	rm -f *~ \#*\# *.o morpho



morpho: $(OBJS)
	$(CXX) $(CXXFLAGS)  ${LIB_PATH} ${OBJS} -o $@ ${LIB}


%.o: %.cc
	$(CXX) $(CXXFLAGS)  -I/work/da-mota/milena -c $< -o $@

