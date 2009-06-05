.PHONY: ref-guide ref-guide-html

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(DOC_SRCDIR):$(DOC_BUILDDIR):$(OUTPUTS_SRCDIR):$(IMG_SRCDIR):


ref-guide:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex


ref_guide.pdf: ref-guide


ref-guide-html: 
	chmod u+x $(DOC_SRCDIR)/tools/todoxygen.sh
	$(DOC_SRCDIR)/tools/todoxygen.sh $(srcdir)/ref_guide.tex $(DOC_BUILDDIR)/ref_guide $(DOC_SRCDIR)


clean-local:
	rm -f ref_guide.aux ref_guide.toc ref_guide.log ref_guide.bbl ref_guide.out
	rm -f *~ *blg *.lot
	rm -f ref_guide.pdf
	rm -f *.haux *.hh *.html *.htoc

EXTRA_DIST = 		\
  ref_guide.tex 	\
  ref_guide.pdf

