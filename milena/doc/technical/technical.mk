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

.PHONY: technical technical-html technical-pdf

technical_TEXINPUTS ="$(DOC_SRCDIR):$(OUTPUTS_SRCDIR):$(srcdir):\
$(SPLIT_OUTPUTS_SRCDIR):$(IMG_SRCDIR):$(SPLIT_EXAMPLES_SRCDIR):"


technical: technical-html technical-pdf

technical_dir = $(doc_dir)/technical

# FIXME: Check these dependencies.  And it'd be better to depend on
# actual files rather than timestamps correponding to a bunch of
# files.
technical_dependencies = $(doc_dir)/figures.stamp

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
# This is not a bug: TECHNICAL_HH is meant to have a `.hh'
# extension, since it is later parsed by Doxygen, which complains
# about `.html' files.
TECHNICAL_HH = $(technical_dir)/technical.hh
technical-html: $(TECHNICAL_HH)
$(TECHNICAL_HH): $(technical_dir)/technical.tex $(technical_dependencies)
	$(doc_dir)/tools/todoxygen.sh $< $(technical_dir) $(doc_dir)


# Final product.
TECHNICAL_PDF = $(technical_dir)/technical.pdf
technical-pdf: $(TECHNICAL_PDF)
$(TECHNICAL_PDF): $(technical_dir)/technical.tex $(technical_dependencies)
	TEXINPUTS=$(technical_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(technical_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(technical_TEXINPUTS) pdflatex $<	\
	test "x$(technical_dir)" != x.			\
	  && mv -f technical.pdf $(technical_dir)

dist_doc_DATA += $(TECHNICAL_PDF)

EXTRA_DIST +=					\
  $(technical_dir)/technical.tex		\
  $(TECHNICAL_HH)

# FIXME: Remove `tutorial.aux tutorial.toc tutorial.log tutorial.bbl
# tutorial.out *blg *.lot' when texi2dvi is used.
#
# FIXME: Why is $(TUTORIAL_PDF) listed here?  It should be
# automatically cleaned.  I'm not sure either about the rest of
# CLEANFILES.
CLEANFILES +=								\
  technical.aux technical.toc technical.log technical.bbl technical.out	\
  *blg *.lot								\
  $(TECHNICAL_PDF)							\
  *.haux *.hh *.html *.htoc						\
  technical.html							\
  technical.idx								\
  $(TECHNICAL_HH)
