CXX = llvm-g++
CXXFLAGS = -DNDEBUG -O4

all: test.cc
	${CXX} -I../../../ `Magick++-config --cppflags --cxxflags --ldflags --libs` ${CXXFLAGS} test.cc -o test
