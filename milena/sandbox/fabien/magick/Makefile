CXX = g++
CXXFLAGS = -DNDEBUG -O1
LIBS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
INC = -I../../../


magick: magick.cc
	${CXX} ${INC} ${LIBS} $^ -o mln_magick

save: save.cc
	${CXX} ${INC} ${LIBS} $^ -o magick_save

tiled: tiled_image.cc
	${CXX} ${INC} ${LIBS} $^ -o tiled
