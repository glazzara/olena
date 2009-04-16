include ../igr/Makefile.rules

all: mask dump

mask: dicom_mask.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o dicom_mask

dump: dicom2dump.cc
	${CXX} -I../../../ ${CXXFLAGS} ${DICOM} $^ -o dicom2dump

pgm: dicom2pgm.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o dicom2pgm
