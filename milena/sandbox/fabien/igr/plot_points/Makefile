include ../Makefile.rules

all: main.cc
	${CXX} -I../../../../ ${CXXFLAGS} $^ -o plot_all_points
	
plot_points_of_label: plot_points_of_label.cc
	${CXX} -I../../../../ ${CXXFLAGS} $^ -o plot_points_all_label
	

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
