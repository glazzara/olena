SUBDIRS = samples

.PHONY: tuto tuto_html

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(TUTORIAL_SRCDIR):$(TUTORIAL_BUILDDIR):$(OUTPUTS_SRCDIR):

tuto: data fig-convert
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex

tuto_html: data fig-convert 
	chmod u+x $(TUTORIAL_SRCDIR)/tools/todoxygen.sh
	$(TUTORIAL_SRCDIR)/tools/todoxygen.sh $(TUTORIAL_SRCDIR)/tutorial.tex $(TUTORIAL_BUILDDIR)

data:
	make -C samples data

fix-refdata:
	make -C samples fix-refdata

fig-convert:
	@failcom='exit 1'; 							\
	list="$(FIGURES_BUILDDIR)/*.p*m"; for img in $$list; do 		\
	  echo "Converting $$img to png"; 					\
	  name=`basename $$img` 						\
	  ext=`echo $$name | cut -d '.' -f 2`; 					\
	  convert $$img $(FIGURES_BUILDDIR)/`basename $$img $$ext`png		\
	    || eval $$failcom; 							\
	done; 

clean-local:
	rm -f tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out
	rm -f *~ *blg *.lot
	rm -f tutorial.pdf
	rm -f *.haux *.hh *.html *.htoc
