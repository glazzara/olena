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

EPSS = $(PNGS:png=eps)

# FIXME: we want to check if 'figures' exists only once!
.png.eps:
	test -d figures || mkdir figures
	convert $< $@


.PHONY: white-paper white-paper-html white-paper-pdf

white-paper: white-paper-pdf white-paper-html


WHITE_PAPER_HTML = white_paper_html/index.html
white-paper-html: $(WHITE_PAPER_HTML)
$(WHITE_PAPER_HTML): $(srcdir)/white_paper.tex $(EPSS)
	test -d white_paper_html || mkdir white_paper_html
	TEXINPUTS=$(TEXINPUTS) hevea -O -fix $< -o $@
	$(top_srcdir)/milena/doc/tools/clearbanner.sh $@


WHITE_PAPER_PDF = white_paper.pdf
white-paper-pdf: $(WHITE_PAPER_PDF)
$(WHITE_PAPER_PDF):
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/white_paper.tex


dist_doc_DATA =					\
  $(WHITE_PAPER_HTML)				\
  $(WHITE_PAPER_PDF)

EXTRA_DIST =					\
  white_paper.tex				\
  $(PNGS)					\
  $(EPSS)

CLEANFILES =					\
  white_paper_html				\
  white_paper_image.*				\
  white_paper.pdf				\
  *.log *.idx *.out *.aux			\
  figures
