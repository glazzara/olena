# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.
#

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS=$(DOC_SRCDIR):$(srcdir):

PNGS = 					\
  figures/house.png 			\
  figures/house_rag.png 		\
  figures/house_wshed.png 		\
  figures/house_wshed_mean_colors.png

#FIXME: we want to check if 'figures' exists only once!
.png.eps:
	test -d figures || mkdir figures
	convert $< $@


.PHONY: white-paper white-paper-html white-paper-pdf

white-paper: white-paper-pdf white-paper-html


white-paper-pdf: white_paper.pdf

white-paper-html: $(PNGS:png=eps)
	test -d white_paper_html || mkdir white_paper_html
	TEXINPUTS=$(TEXINPUTS) hevea -O -fix $(srcdir)/white_paper.tex -o white_paper_html/index.html
	chmod +x $(top_srcdir)/milena/doc/tools/clearbanner.sh
	$(top_srcdir)/milena/doc/tools/clearbanner.sh white_paper_html/index.html


white_paper.pdf:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex

clean-local:
	rm -Rf white_paper_html
	rm -f white_paper_image.*
	rm -f white_paper.pdf
	rm -f *.log *.idx *.out *.aux
	rm -Rf figures

EXTRA_DIST = 		\
  white_paper.tex 	\
  white_paper.pdf 	\
  $(PNGS)

