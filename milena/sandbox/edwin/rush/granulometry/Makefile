include makefile.rules
TARGET=test

test_SRC=test.cc
test_OBJS=${test_SRC:.cc=.o}

OLENADIR=$(MLN_DIR)/..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -W -Wall


CXXFLAGS += $(if $(DEBUG), -g -ggdb, -DNDEBUG\
	$(if $(RELEASE), -O3, -O1))

CXX=g++
LD=g++
LDFLAGS=

all: $(TARGET)


$(TARGET): $($(TARGET)_OBJS) $($(TARGET)_SRC)
	$(LD) $(LDFLAGS) -o $@ $($(TARGET)_OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

depend:
	makedepend -- $(CXXFLAGS) -- -v $($(TARGET)_SRC)

clean:
	rm -f *.o $(TARGET)
