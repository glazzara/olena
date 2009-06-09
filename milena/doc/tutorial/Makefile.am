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

tutorial:
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(srcdir)/tutorial.tex


tutorial.pdf: tutorial


tutorial-html: 
	chmod u+x $(DOC_SRCDIR)/tools/todoxygen.sh
	$(DOC_SRCDIR)/tools/todoxygen.sh $(srcdir)/tutorial.tex $(DOC_BUILDDIR)/tutorial $(DOC_SRCDIR)


clean-local:
	rm -f tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out
	rm -f *~ *blg *.lot
	rm -f tutorial.pdf
	rm -f *.haux *.hh *.html *.htoc


EXTRA_DIST = 		\
  tutorial.tex 		\
  tutorial.pdf

