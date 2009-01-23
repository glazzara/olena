CFLAGS=#-O3 -DNDEBUG

all: sd on_set on_fun


sd: self_dual_reconstruction.o
	g++ self_dual_reconstruction.o -o sd

on_set: reconstructions_on_set.o
	g++ reconstructions_on_set.o -o on_set

on_fun: reconstructions_on_function.o
	g++ reconstructions_on_function.o -o on_fun

clean:
	rm -f sd on_set on_fun reconstructions_on_set.o reconstructions_on_function.o self_dual_reconstruction.o

%.o : %.cc
	g++ -c -I. -I../../.. $(CFLAGS) $<

reconstructions_on_function.o: reconstructions_on_function.cc \
  reconstructions_on_function.hh canvas/reconstruction_on_function.hh
reconstructions_on_set.o: reconstructions_on_set.cc \
  reconstructions_on_set.hh canvas/reconstruction_on_set.hh
self_dual_reconstruction.o: self_dual_reconstruction.cc \
  self_dual_reconstruction.hh canvas/self_dual_reconstruction.hh \
  reconstructions_on_function.hh canvas/reconstruction_on_function.hh
