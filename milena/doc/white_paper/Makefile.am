include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(DOC_SRCDIR):$(srcdir)/figures:

PNGS = 					\
  figures/house.png 			\
  figures/house_rag.png 		\
  figures/house_wshed.png 		\
  figures/house_wshed_mean_colors.png

.png.eps:
	convert $< $@

.PHONY: white-paper white-paper-html

white-paper:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex

white-paper.pdf: white-paper

white-paper-html: $(PNGS:png=eps)
	mkdir -p white_paper_html
	TEXINPUTS=$(TEXINPUTS) hevea -O -fix -I $(srcdir)/figures $(srcdir)/white_paper.tex -o white_paper_html/index.html
	chmod +x $(top_srcdir)/milena/doc/tools/clearbanner.sh
	$(top_srcdir)/milena/doc/tools/clearbanner.sh white_paper_html/index.html

clean-local:
	rm -Rf white_paper_html
	rm -f *.eps
	rm -f white_paper_image.*
	rm -f white_paper.pdf
	rm -f *.log *.idx *.out *.aux

EXTRA_DIST = 		\
  white_paper.tex 	\
  white_paper.pdf 	\
  $(PNGS)

