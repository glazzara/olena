.PHONY: tutorial tuto-html

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(DOC_SRCDIR):$(DOC_BUILDDIR):$(OUTPUTS_SRCDIR):$(IMG_SRCDIR):

tutorial:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/tutorial.tex

tuto-html: 
	chmod u+x $(DOC_SRCDIR)/tools/todoxygen.sh
	$(DOC_SRCDIR)/tools/todoxygen.sh $(srcdir)/tutorial.tex $(DOC_BUILDDIR)/tutorial

clean-local:
	rm -f tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out
	rm -f *~ *blg *.lot
	rm -f tutorial.pdf
	rm -f *.haux *.hh *.html *.htoc

EXTRA_DIST = 		\
tutorial.tex 		\
tutorial.pdf

