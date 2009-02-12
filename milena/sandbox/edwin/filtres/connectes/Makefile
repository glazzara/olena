TARGET=filter
REF=leveling
SRC=filter.cc
SRC_REF=leveling.cc
OBJS=${SRC:.cc=.o}
OBJS_REF=${SRC_REF:.cc=.o}

OLENADIR=../../../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -DNDEBUG -O3 -ffast-math
CXX=g++
LD=g++
LDFLAGS=

all: $(TARGET)

$(TARGET): $(OBJS) $(SRC)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)


$(REF): $(OBJS_REF) $(SRC_REF)
	$(LD) $(LDFLAGS) -o $@ $(OBJS_REF)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(REF) $(TARGET)