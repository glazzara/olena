GCCFLAGS = -DNDEBUG -O1
LLVMFLAGS = -DNDEBUG -O4

all: gcc llvm

gcc: test.cc
	g++ -I../../../ `Magick++-config --cppflags --cxxflags --ldflags --libs` ${GCCFLAGS} test.cc -o test_gcc

llvm: test.cc
	llvm-g++ -I../../../ `Magick++-config --cppflags --cxxflags --ldflags --libs` ${LLVMFLAGS} test.cc -o test_llvm
