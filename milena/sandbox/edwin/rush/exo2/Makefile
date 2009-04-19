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

OUT_IMG=superpose.pgm mean.pgm gradient.pgm closing.pgm

all: wst_from_closing

%: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@


clean:
	rm -f $(OUT_IMG)
	rm -f wst_from_closing


%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<