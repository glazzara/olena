CXX = llvm-g++
CXXFLAGS = -DNDEBUG -O4
LIBS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
INC = -I../../../

all: magick

magick: magick.cc
	${CXX} ${INC} ${LIBS} $^ -o mln_magick

save: save.cc
	${CXX} ${INC} ${LIBS} $^ -o magick_save
