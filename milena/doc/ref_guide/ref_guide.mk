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

.PHONY: ref-guide ref-guide-html ref-guide-pdf

ref_guide_TEXINPUTS = "$(DOC_SRCDIR):$(OUTPUTS_SRCDIR):\
$(SPLIT_OUTPUTS_SRCDIR):$(IMG_SRCDIR):$(SPLIT_EXAMPLES_SRCDIR):"


ref-guide: ref-guide-html ref-guide-pdf

ref_guide_dir = $(doc_dir)/ref_guide

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
# This is not a bug: REF_GUIDE_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
REF_GUIDE_HH = $(ref_guide_dir)/ref_guide.hh
ref-guide-html: $(REF_GUIDE_HH)
$(REF_GUIDE_HH): $(ref_guide_dir)/ref_guide.tex $(doc_dir)/figures.stamp
	$(doc_dir)/tools/todoxygen.sh $< $(ref_guide_dir) $(doc_dir)


# Final product.
REF_GUIDE_PDF = $(ref_guide_dir)/ref_guide.pdf
ref-guide-pdf: $(REF_GUIDE_PDF)
$(REF_GUIDE_PDF): $(ref_guide_dir)/ref_guide.tex $(doc_dir)/figures.stamp
	TEXINPUTS=$(ref_guide_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(ref_guide_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(ref_guide_TEXINPUTS) pdflatex $<	\
	test x"$(ref_guide_dir)" != x.			\
	  && mv -f ref_guide.pdf $(ref_guide_dir)

dist_doc_DATA += $(REF_GUIDE_PDF)

EXTRA_DIST +=					\
  $(ref_guide_dir)/ref_guide.tex		\
  $(REF_GUIDE_HH)

# FIXME: Remove `ref_guide.aux ref_guide.toc ref_guide.log
# ref_guide.bbl ref_guide.out *blg *.lot' when texi2dvi is used.
#
# FIXME: Why is $(REF_GUIDE_PDF) listed here?  It should be
# automatically cleaned.  I'm not sure either about the rest of
# CLEANFILES.
CLEANFILES +=								\
  ref_guide.aux ref_guide.toc ref_guide.log ref_guide.bbl ref_guide.out	\
  *blg *.lot								\
  $(REF_GUIDE_PDF)							\
  *.haux *.hh *.html *.htoc						\
  ref_guide.html							\
  ref_guide.idx								\
  $(REF_GUIDE_HH)
