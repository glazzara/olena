TARGET=algebraic
SRC=algebraic.cc
OBJS=${SRC:.cc=.o}

OLENADIR=../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -DNDEBUG -O3 -ffast-math
CXX=g++
LD=g++
LDFLAGS=

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<
