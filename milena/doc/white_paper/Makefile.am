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

WHITE_PAPER_HTML = $(srcdir)/white_paper_html/index.html
white-paper-html: $(WHITE_PAPER_HTML)
$(WHITE_PAPER_HTML): white_paper.tex $(EPSS)
	test -d $(srcdir)/white_paper_html || mkdir $(srcdir)/white_paper_html
	TEXINPUTS=$(TEXINPUTS) hevea -O -fix $< -o $@
	$(top_srcdir)/milena/doc/tools/clearbanner.sh $@


WHITE_PAPER_PDF = $(srcdir)/white_paper.pdf
white-paper-pdf: $(WHITE_PAPER_PDF)
$(WHITE_PAPER_PDF): white_paper.tex $(PNGS)
	TEXINPUTS=$(TEXINPUTS) pdflatex $<
	TEXINPUTS=$(TEXINPUTS) pdflatex $<
	TEXINPUTS=$(TEXINPUTS) pdflatex $<	\
	  && mv -f white_paper.pdf $(srcdir)


dist_doc_DATA =					\
  $(WHITE_PAPER_HTML)				\
  $(WHITE_PAPER_PDF)

EXTRA_DIST =					\
  white_paper.tex				\
  $(PNGS)					\
  $(EPSS)

CLEANFILES =					\
  white_paper_image.*				\
  white_paper.pdf				\
  *.log *.idx *.out *.aux

clean-local:
	-rm -rf white_paper_html figures


#<<lrde
# Pretend the documentation is up-to-date.
.PHONY: fake-doc
fake-doc:
	test -d figures || mkdir figures
	touch $(EPSS)
	test -d $(srcdir)/white_paper_html || mkdir $(srcdir)/white_paper_html
	touch $(WHITE_PAPER_HTML)
	touch $(WHITE_PAPER_PDF)

# The converse of the previous rule, voiding the timestamps.
.PHONY: void-doc
void-doc:
	touch $(srcdir)/white_paper.tex
#>>
