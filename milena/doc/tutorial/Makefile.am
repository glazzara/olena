SUBDIRS = samples

.PHONY: tuto

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(TUTORIAL_SRCDIR):$(TUTORIAL_BUILDDIR):

tuto:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex

tuto_html:
	chmod u+x $(TUTORIAL_SRCDIR)/todoxygen.sh
	$(TUTORIAL_SRCDIR)/todoxygen.sh $(TUTORIAL_SRCDIR)/tutorial.tex $(TUTORIAL_BUILDDIR)

clean-local:
	rm -f *.aux *.toc *.log *.bbl *blg *.lot *.out *~
	rm -f tutorial.pdf
	rm *.haux *.hh *.html *.htoc
