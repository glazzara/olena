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

TEXINPUTS = $(DOC_SRCDIR):$(OUTPUTS_SRCDIR):$(IMG_SRCDIR):


tutorial: tutorial-html tutorial-pdf

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

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TUTORIAL_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
TUTORIAL_HH = $(srcdir)/tutorial.hh
tutorial-html: $(TUTORIAL_HH)
$(TUTORIAL_HH): tutorial.tex $(srcdir)/../figures.stamp
	$(DOC_SRCDIR)/tools/todoxygen.sh		\
	  $< $(DOC_SRCDIR)/tutorial $(DOC_SRCDIR)

# Final product.
TUTORIAL_PDF = $(srcdir)/tutorial.pdf
tutorial-pdf: $(TUTORIAL_PDF)
$(TUTORIAL_PDF): tutorial.tex $(srcdir)/../figures.stamp
	TEXINPUTS=$(TEXINPUTS) pdflatex $<
	TEXINPUTS=$(TEXINPUTS) pdflatex $<
	TEXINPUTS=$(TEXINPUTS) pdflatex $<		\
	test "$(top_srcdir)" == "$(top_builddir)"	\
		|| mv -f tutorial.pdf $(srcdir)

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
  *blg *.lot								\
  $(TUTORIAL_PDF)							\
  *.haux *.hh *.html *.htoc						\
  tutorial.haux								\
  tutorial.html								\
  tutorial.idx								\
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
