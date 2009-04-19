include makefile.rules
TARGET=export
#sharp_SRC=sharp.cc
#sharp_OBJS=${sharp_SRC:.cc=.o}

export_SRC=export.cc
export_OBJS=${export_SRC:.cc=.o}

OLENADIR=$(MLN_DIR)/..
MILENADIR=$(MLN_DIR)

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
	#rm -f *.pbm
	#find  -name "*.pgm" \! -regex ".*/affiche2?.pgm" -delete
# DO NOT DELETE