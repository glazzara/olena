include ../Makefile.rules


linear: linear.cc
	${CXX} -I../../../../ ${CXXFLAGS} $^ -o linear
	
median: median.cc
	${CXX} -I../../../../ ${DICOM} ${CXXFLAGS} $^ -o median
	
morpho: morpho.cc
	${CXX} -I../../../../ ${DICOM} ${MAGICK} ${CXXFLAGS} $^ -o morpho

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
	rm linear median morpho
