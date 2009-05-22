include ../Makefile.rules

all: main.cc
	${CXX} -I../../../../ ${CXXFLAGS} $^ -o point_filtering
	

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
	rm point_filtering
