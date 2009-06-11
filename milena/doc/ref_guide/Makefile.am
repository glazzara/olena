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


ref-guide: $(FIGURES_SRCDIR)/* $(OUTPUTS_SRCDIR)/* ref-guide-pdf ref-guide-html


ref-guide-pdf: ref_guide.pdf


ref_guide.pdf:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/ref_guide.tex


ref-guide-html: 
	chmod u+x $(DOC_SRCDIR)/tools/todoxygen.sh
	$(DOC_SRCDIR)/tools/todoxygen.sh $(srcdir)/ref_guide.tex $(DOC_BUILDDIR)/ref_guide $(DOC_SRCDIR)


clean-local:
	rm -f ref_guide.aux ref_guide.toc ref_guide.log ref_guide.bbl ref_guide.out
	rm -f *~ *blg *.lot
	rm -f ref_guide.pdf
	rm -f *.haux *.hh *.html *.htoc

EXTRA_DIST = 		\
  ref_guide.tex 	\
  ref_guide.pdf

