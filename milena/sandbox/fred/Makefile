TARGET=meantest
OBJS=meantest.o

OLENADIR=../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -DNDEBUG -O2 -ffast-math
CXX=g++
LD=g++
LDFLAGS=
RM=rm

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET)

clean:
	$(RM) -f $(TARGET) *.o *~