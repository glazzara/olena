CXX = g++
CXXFLAGS = -DNDEBUG -O1
LIBS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
INC = -I../../../ -I../../../../../

all: tiled2d disk_ima

tiled2d: tiled2d.cc
	${CXX} ${CXXFLAGS} ${INC} $^ -o tiled2d

disk_ima: disk_ima.cc
	${CXX} ${CXXFLAGS} ${INC} $^ -o disk_ima

ima2d: ima2d.cc
	${CXX} ${CXXFLAGS} ${INC} $^ -o ima2d
