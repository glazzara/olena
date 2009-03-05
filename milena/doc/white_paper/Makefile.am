include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(top_srcdir)/milena/doc/white_paper/figures:

white-paper:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex

