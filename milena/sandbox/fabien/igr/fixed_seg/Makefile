include ../Makefile.rules

all: main.cc
	${CXX} -I../../../../ ${CXXFLAGS} $^ -o fixed_seg
	

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
	rm seg_fixed
