all: minsurf keep_object

minsurf: main.cc
	g++ -O2 -DNDEBUG -I ../../.. main.cc -o minsurf

keep_object: keep_object.cc
	g++  -O2 -DNDEBUG -I ../../.. keep_object.cc -o keep_object

debug:
	g++ -g3 -ggdb3 -I ../../.. main.cc -o minsurf

check: minsurf keep_object
	./check.sh

clean:
	rm -rf keep_object
	rm -rf minsurf
