TARGETS=exo3_1 exo3_2 inverse

OLENADIR=../../../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -W -Wall -O2
CXX=g++
LD=g++
LDFLAGS=
RM=rm

all: $(TARGETS)

exo3_1: exo3_1.o
exo3_2: exo3_2.o
inverse: inverse.o

$(TARGETS):
	$(LD) $(LDFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) -f $(TARGETS) *.o *~
