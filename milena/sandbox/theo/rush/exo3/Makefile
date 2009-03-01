TARGETS=exo3_fun_area exo3_fun_generator assignable

OLENADIR=../../../../..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS+=-I$(MILENADIR) -I./ -W -Wall -O2
CXX=g++
LD=g++
LDFLAGS=
RM=rm

all: $(TARGETS)

exo3_fun_area: exo3_fun_area.o
exo3_fun_generator: exo3_fun_generator.o
assignable: assignable.o

$(TARGETS):
	$(LD) $(LDFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) -f $(TARGETS) *.o *~
