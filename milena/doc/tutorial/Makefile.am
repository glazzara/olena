SUBDIRS = samples

.PHONY: tuto

TUTO_DIR = "milena/doc/tutorial"
TEXINPUTS = $(top_srcdir)/$(TUTO_DIR):$(top_builddir)/$(TUTO_DIR):

tuto:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(top_srcdir)/$(TUTO_DIR)/tutorial.tex

tuto_html:
	chmod u+x $(top_srcdir)/$(TUTO_DIR)/todoxygen.sh
	$(top_srcdir)/$(TUTO_DIR)/todoxygen.sh $(top_srcdir)/$(TUTO_DIR)/tutorial.tex $(top_builddir)/$(TUTO_DIR)

clean-local:
	rm -f *.aux *.toc *.log *.bbl *blg *.lot *.out *~
	rm -f tutorial.pdf
	rm *.haux *.hh *.html *.htoc
