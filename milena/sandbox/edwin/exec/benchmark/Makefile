-include makefile.rules

MLN_DIR = ~/olena/trunk/milena
CXXFLAGS += -W -Wall
CXXFLAGS += $(if $(DEBUG), -g -ggdb, -DNDEBUG $(if $(RELEASE), -O3, -O1))
MY_CXXFLAGS := $(CXXFLAGS) -I $(MLN_DIR)/sandbox/edwin -I $(MLN_DIR)
CXXFLAGS += -I $(MLN_DIR)
CC = g++
LDFLAGS =

all: oldv newv

newv.o: newv.cc
	$(CXX) $(MY_CXXFLAGS) -c $<

clean:
	rm -f *.o

newv: newv.o

oldv: oldv.o
