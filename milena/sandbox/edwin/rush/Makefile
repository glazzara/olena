include makefile.rules


OLENADIR=$(MLN_DIR)/..
MILENADIR=$(MLN_DIR)

CXXFLAGS = -I$(MILENADIR) -I./ -W -Wall
CXXFLAGS += $(if $(DEBUG), -g -ggdb, -DNDEBUG\
	$(if $(RELEASE), -O3, -O1))
CXX=g++
LD=g++
LDFLAGS=
SRC=


GRADIENT_SRC = $(MLN_DIR)/sandbox/theo/exec/color_internal_gradient.cc
LEAVES_SRC = get_leaves.cc


all: leaves gradient


leaves: $(LEAVES_SRC)
	$(CXX) $(CXXFLAGS) $< -o leaves

gradient: $(GRADIENT_SRC)
	$(CXX) $(CXXFLAGS) $< -o gradient


clean:
	rm -f leaves gradient


%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<