CXX = g++
CXXFLAGS = -DNDEBUG -O1
LIBS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
INC = -I../../../ -I../../../../../

all: tiled2d

tiled2d: tiled2d.cc
	${CXX} ${CXXFLAGS} ${INC} $^ -o tiled2d
