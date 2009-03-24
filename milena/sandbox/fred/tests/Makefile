TARGETS=fun.bin thru.bin cos.bin

OLENADIR=../../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I../ -DNDEBUG -O1
CXX=g++
LD=g++
LDFLAGS=
RM=rm

%.bin: %.o
	$(LD) $(LDFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGETS)

clean:
	$(RM) -f $(TARGETS) *.o *~
