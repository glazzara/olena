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
# FIXME: Missing dependencies on images from $(FIGURES_BUILDDIR).
$(REF_GUIDE_HH): $(FIGURES_BUILDDIR)
	$(DOC_SRCDIR)/tools/todoxygen.sh $(srcdir)/ref_guide.tex $(DOC_BUILDDIR)/ref_guide $(DOC_SRCDIR)


# Final product.
REF_GUIDE_PDF = ref_guide.pdf
ref-guide-pdf: $(REF_GUIDE_PDF)
# FIXME: Missing dependencies on images from $(FIGURES_BUILDDIR).
$(REF_GUIDE_PDF): $(FIGURES_BUILDDIR)
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex


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
