#							-*- Automake -*-

# Copyright (C) 2008, 2009, 2010 EPITA Research and Development
# Laboratory (LRDE).
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

.PHONY: tutorial tutorial-html tutorial-pdf

tutorial_TEXINPUTS = $(DOC_SRCDIR):$(OUTPUTS_SRCDIR):$(IMG_SRCDIR):


tutorial: tutorial-html tutorial-pdf

tutorial_dir = $(doc_dir)/tutorial

# FIXME: Check these dependencies.
# 1. They might be redundant.
# 2. It'd be better to depend on actual files rather than timestamps
#    correponding to a bunch of files.
tutorial_dependencies =				\
  $(doc_dir)/split-examples.stamp		\
  $(doc_dir)/split-outputs.stamp		\
  $(doc_dir)/figures.stamp

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
TUTORIAL_HH = $(tutorial_dir)/tutorial.hh
tutorial-html: $(TUTORIAL_HH)
$(TUTORIAL_HH): $(tutorial_dir)/tutorial.tex $(tutorial_dependencies)
	$(doc_dir)/tools/todoxygen.sh $< $(tutorial_dir) $(doc_dir)

# Final product.
TUTORIAL_PDF = $(tutorial_dir)/tutorial.pdf
tutorial-pdf: $(TUTORIAL_PDF)
$(TUTORIAL_PDF): $(tutorial_dir)/tutorial.tex $(tutorial_dependencies)
	TEXINPUTS=$(tutorial_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(tutorial_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(tutorial_TEXINPUTS) pdflatex $<	\
	test x"$(tutorial_dir)" != x.			\
	  && mv -f tutorial.pdf $(tutorial_dir)

dist_doc_DATA += $(TUTORIAL_PDF)

EXTRA_DIST +=					\
  $(tutorial_dir)/tutorial.tex			\
  $(TUTORIAL_HH)

# FIXME: Remove `tutorial.aux tutorial.toc tutorial.log tutorial.bbl
# tutorial.out *blg *.lot' when texi2dvi is used.
#
# FIXME: Why is $(TUTORIAL_PDF) listed here?  It should be
# automatically cleaned.  I'm not sure either about the rest of
# CLEANFILES.
CLEANFILES +=								\
  tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out	\
  *blg *.lot								\
  $(TUTORIAL_PDF)							\
  *.haux *.hh *.html *.htoc						\
  tutorial.html								\
  tutorial.idx								\
  $(TUTORIAL_HH)
