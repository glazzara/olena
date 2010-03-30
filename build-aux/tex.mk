## tex.mk 					-*- Makefile-Automake -*-

## A small subset of LRDE's share/make/tex.mk.

## Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.

build_aux_dir = $(top_srcdir)/build-aux

TEXI2DVI = $(build_aux_dir)/texi2dvi
TEXI2DVI_FLAGS = --tidy --build-dir=tmp.t2d --batch


TEXI2PDF = $(TEXI2DVI) --pdf
TEXI2PDF_FLAGS = $(TEXI2DVI_FLAGS)

# FIXME: Switch back to HeVeA, since we currently rely on its outputs
# to produce intermediate products later read by Doxygen.  When these
# intermediate files are produced directly (from LaTeX sources),
# switch again to TeX4ht.
TEXI2HTML = $(TEXI2DVI) --html # --tex4ht
TEXI2HTML_FLAGS = $(TEXI2DVI_FLAGS)

SUFFIXES = .tex .pdf

.tex.pdf:
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) -o $@ $<

tex-mostlyclean:
	rm -rf tmp.t2d
.PHONY: tex-mostlyclean
# mostlyclean-local is an Automake special target.
mostlyclean-local: tex-mostlyclean
.PHONY: mostlyclean-local
