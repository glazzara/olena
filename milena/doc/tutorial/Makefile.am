# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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

.PHONY: tutorial tutorial-html

include $(top_srcdir)/milena/doc/doc.mk

TEXINPUTS = $(DOC_SRCDIR):$(DOC_BUILDDIR):$(OUTPUTS_SRCDIR):$(IMG_SRCDIR):


tutorial: tutorial-html tutorial-pdf

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TUTORIAL_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
TUTORIAL_HH = tutorial.hh
tutorial-html: $(TUTORIAL_HH)
$(TUTORIAL_HH): tutorial.tex $(srcdir)/../figures.stamp
	$(DOC_SRCDIR)/tools/todoxygen.sh		\
	  $< $(DOC_BUILDDIR)/tutorial $(DOC_SRCDIR)


# Final product.
TUTORIAL_PDF = tutorial.pdf
tutorial-pdf: $(TUTORIAL_PDF)
$(TUTORIAL_PDF): tutorial.tex $(srcdir)/../figures.stamp
	TEXINPUTS=$(TEXINPUTS) pdflatex $<
	TEXINPUTS=$(TEXINPUTS) pdflatex $<
	TEXINPUTS=$(TEXINPUTS) pdflatex $<

# FIXME: Regenerating figures.stamp requires make to go back to the
# parent directory.  We already do the opposite (descending from
# milena/doc/ to milena/doc/tutorial/Makefile in milena/doc/ to update
# tutorial.hh).  This is not sound.  We probably want to put together
# somes of these files, and maybe get rid of some directories, or at
# least move most of the Makefile machinery into
# milena/doc/Makefile.am.
$(srcdir)/../figures.stamp:
	cd .. && $(MAKE) $(AM_MAKEFLAGS) fig-convert

dist_doc_DATA = $(TUTORIAL_PDF)

EXTRA_DIST =					\
  tutorial.tex					\
  $(TUTORIAL_HH)

CLEANFILES =								\
  tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out	\
  *~ *blg *.lot								\
  $(TUTORIAL_PDF)							\
  *.haux *.hh *.html *.htoc						\
  tutorial.haux								\
  tutorial.html								\
  $(TUTORIAL_HH)


#<<lrde
# Pretend the documentation is up-to-date.
.PHONY: fake-doc
fake-doc:
	touch $(TUTORIAL_HH)
	touch $(TUTORIAL_PDF)

# The converse of the previous rule, voiding the timestamps.
.PHONY: void-doc
void-doc:
	touch $(srcdir)/tutorial.tex
#>>
