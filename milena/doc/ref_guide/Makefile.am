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

.PHONY: ref-guide ref-guide-html

include $(top_srcdir)/milena/doc/doc.mk

TEXINPUTS ="$(DOC_SRCDIR):$(DOC_BUILDDIR):$(OUTPUTS_SRCDIR):\
$(SPLIT_OUTPUTS_SRCDIR):$(IMG_SRCDIR):$(SPLIT_EXAMPLES_SRCDIR):"


ref-guide: ref-guide-html ref-guide-pdf


# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: REF_GUIDE_HH is meant to have a `.hh'
# extension, since it is later parsed by Doxygen, which complains
# about `.html' files.
REF_GUIDE_HH = ref_guide.hh
ref-guide-html: $(REF_GUIDE_HH)
$(REF_GUIDE_HH): ref_guide.tex $(srcdir)/../figures.stamp
	$(DOC_SRCDIR)/tools/todoxygen.sh		\
	  $< $(DOC_BUILDDIR)/ref_guide $(DOC_SRCDIR)


# Final product.
REF_GUIDE_PDF = ref_guide.pdf
ref-guide-pdf: $(REF_GUIDE_PDF)
$(REF_GUIDE_PDF): ref_guide.tex $(srcdir)/../figures.stamp
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

dist_doc_DATA = $(REF_GUIDE_PDF)

EXTRA_DIST =					\
  ref_guide.tex					\
  $(REF_GUIDE_HH)

CLEANFILES =								\
  ref_guide.aux ref_guide.toc ref_guide.log ref_guide.bbl ref_guide.out	\
  *~ *blg *.lot								\
  $(REF_GUIDE_PDF)							\
  *.haux *.hh *.html *.htoc						\
  ref_guide.html							\
  $(REF_GUIDE_HH)


#<<lrde
# Pretend the documentation is up-to-date.
.PHONY: fake-doc
fake-doc:
	touch $(REF_GUIDE_HH)
	touch $(REF_GUIDE_PDF)

# The converse of the previous rule, voiding the timestamps.
.PHONY: void-doc
void-doc:
	touch $(srcdir)/ref_guide.tex
#>>
