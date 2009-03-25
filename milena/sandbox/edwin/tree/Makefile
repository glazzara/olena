include makefile.rules
TARGET=sharp
SRC=sharp.cc
OBJS=${SRC:.cc=.o}

OLENADIR=$(MLN_DIR)/..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -W -Wall


CXXFLAGS += $(if $(DEBUG), -g -ggdb, -DNDEBUG -O1)

CXX=g++
LD=g++
LDFLAGS=

all: clean $(TARGET)


$(TARGET): $(OBJS) $(SRC)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
	#rm -f *.pbm
	#find  -name "*.pgm" \! -regex ".*/affiche2?.pgm" -delete
