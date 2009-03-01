TARGET=exo2
SRC=exo2.cc
OBJS=${SRC:.cc=.o}

OLENADIR=../../../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./

CXXFLAGS += -DNDEBUG -O1

CXX=g++
LD=g++
LDFLAGS=

all: clean $(TARGET)
	chmod +x exo2.sh
	./exo2.sh

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
	rm -f *.pbm
	find  -name "*.pgm" \! -regex ".*/affiche2?.pgm" -delete