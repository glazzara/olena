SRCPGM=$(wildcard *.pgm)
SRCPPM=$(wildcard *.ppm)
SRC=$(wildcard *.cc)
OBJPGM=$(SRCPGM:.pgm=.tex)
OBJPPM=$(SRCPPM:.ppm=.tex)
OBJ=$(SRC:.cc=.o)
EXEC=$(SRC:.cc=) $(SRCPGM:.pgm=.pdf) $(SRCPPM:.ppm=.pdf)
CC=g++
CXXFLAGS=-Wall -W -Werror -I ../../../ -I ../../../../../ -g

%.tex: tikz %.pgm
	tikz $< $@

%.pdf: %.tex
	pdflatex $<

all: $(EXEC)

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf *.log
	rm -rf *.aux
	rm -rf

distclean: clean
	rm -rf $(EXEC)
	rm -rf *.pdf
	rm -rf *.tex

.PHONY: clean distclean

## Depedencies
tikz.o: ../../../mln/io/tikz/save.hh