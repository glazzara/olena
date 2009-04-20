include Makefile.rules
TARGET=boxes

boxes_SRC=boxes.cc
boxes_OBJS=${boxes_SRC:.cc=.o}

OLN_DIR=$(MLN_DIR)/..
SBX_DIR=$(MLN_DIR)/sandbox/

CXXFLAGS=-I $(MLN_DIR) -I ./ -I$(SBX_DIR) -W -Wall


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


distclean: clean
	rm -f Makefile.rules


clean:
	rm -f *.o $(TARGET)
	#rm -f *.pbm
	#find  -name "*.pgm" \! -regex ".*/affiche2?.pgm" -delete
# DO NOT DELETE
