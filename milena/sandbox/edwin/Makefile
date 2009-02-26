TARGET=a.out
SRC=edwin.cc
OBJS=${SRC:.cc=.o}

OLENADIR=../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -DNDEBUG -O1
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