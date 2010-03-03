#							-*- Automake -*-

# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE).
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

.PHONY: white-paper white-paper-html white-paper-pdf

white_paper_dir = $(doc_dir)/white_paper

white_paper_TEXINPUTS = $(DOC_SRCDIR):$(white_paper_dir):

PNGS =								\
  $(white_paper_dir)/figures/house.png				\
  $(white_paper_dir)/figures/house_rag.png			\
  $(white_paper_dir)/figures/house_wshed.png			\
  $(white_paper_dir)/figures/house_wshed_mean_colors.png

EPSS = $(PNGS:png=eps)

# FIXME: we want to check if 'figures' exists only once!
.png.eps:
	test -d figures || mkdir figures
	convert $< $@


white-paper: white-paper-pdf white-paper-html

# FIXME: As in milena/doc/Makefile.am, we should probably strip
# $(srcdir) prefixes from target variables, e.g. instead of:
#
#   FOO = $(srcdir)/foo.pdf
#   $(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# we should use:
#
#   FOO = foo.pdf
#   $(srcdir)/$(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# since it minimizes installation issues (see milena/doc/Makefile.am
# and Vaucanson's doc/Makefile.am).

# FIXME: Distributed products should be generated in the source dir.
# That's actually the case, *but* the current solution is not clean
# and might break sometimes.  The clean approach is to create a
# temporary directory, try to generate the documentation there, and
# move its contents to the source dir in case of success.  If the
# product is a directory, also refresh a timestamp (in the source
# dir).

# FIXME: Use texi2dvi/TeX4ht instead of plain hevea.
WHITE_PAPER_HTML = $(white_paper_dir)/white_paper_html/index.html
white-paper-html: $(WHITE_PAPER_HTML)
$(WHITE_PAPER_HTML): $(white_paper_dir)/white_paper.tex $(EPSS)
	test -d $(white_paper_dir)/white_paper_html	\
	  || mkdir $(white_paper_dir)/white_paper_html
	TEXINPUTS=$(white_paper_TEXINPUTS) hevea -O -fix $< -o $@
	$(doc_dir)/tools/clearbanner.sh $@


WHITE_PAPER_PDF = $(white_paper_dir)/white_paper.pdf
white-paper-pdf: $(WHITE_PAPER_PDF)
$(WHITE_PAPER_PDF): $(white_paper_dir)/white_paper.tex $(PNGS)
	TEXINPUTS=$(white_paper_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(white_paper_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(white_paper_TEXINPUTS) pdflatex $<	\
	test "x$(white_paper_dir)" != x.		\
	  && mv -f white_paper.pdf $(white_paper_dir)


dist_doc_DATA +=				\
  $(WHITE_PAPER_HTML)				\
  $(WHITE_PAPER_PDF)

EXTRA_DIST +=					\
  $(white_paper_dir)/white_paper.tex		\
  $(PNGS)					\
  $(EPSS)

# FIXME: Remove unnecessary patterns.
CLEANFILES +=					\
  white_paper_image.*				\
  white_paper.pdf				\
  *.log *.idx *.out *.aux

# FIXME: Likewise.
clean-local:
	-rm -rf white_paper_html figures
