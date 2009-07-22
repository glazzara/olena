CXX=g++
INCLUDES=-I. -I./binarization
CXX_FLAGS=-W -Wall -DNDEBUG -O3  -ffast-math 
CXXFLAGS = -I.
OBJS= main.o
TARGET=hameli_a
DISTDIR=hameli_a
DISTFILE= Makefile README AUTHORS binarization  \
	check_size_functor.hh main.cc		\
	white.hh white.hxx

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(DEBUG_FLAGS) $^ -o $@

clean:
	rm -f *~ *.o

distclean: clean
	rm -rf $(TARGET)
	rm -rf *_result

dist: distclean
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	chmod 755 $(DISTDIR)
	cp -r -t $(DISTDIR) $(DISTFILE)
	tar czf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)

distcheck: dist
	rm -rf _build
	mkdir _build
	cp $(DISTDIR).tar.gz _build
	cd _build; tar xf $(DISTDIR).tar.gz
	cd _build/$(DISTDIR) ; make

%.o: %.cc
	$(CXX) $(CXX_FLAGS) $(CXXFLAGS) $(INCLUDES) $(DEBUG_FLAGS) -c $<

.PHONY: main.cc 
#