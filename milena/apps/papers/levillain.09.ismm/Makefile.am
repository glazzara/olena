# Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

# Illustrations from the ISMM 2009 paper and presentation (slides).

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

noinst_HEADERS = chain.hh io.hh influence_zones.hh

noinst_PROGRAMS = image2d graph complex classif-graph classif-1complex
image2d_SOURCES = image2d.cc
graph_SOURCES = graph.cc
complex_SOURCES = complex.cc
# This example (implemented both with graph- and 1-complex-based
# images) has been shown in the ISMM 2009 presentation only (not in
# the paper).
classif_graph_SOURCES = classif-graph.cc
classif_1complex_SOURCES = classif-1complex.cc

# Input images.
# FIXME: Move them to milena/img/?
dist_noinst_DATA = seeds.pgm m283-c.off classseedsi.pbm

# Output images.
noinst_DATA =					\
  lena-s.ppm graph-s.png m283-s.off		\
  classwst-graph.ppm classwst-1complex.ppm
MOSTLYCLEANFILES = $(noinst_DATA) graph-s.neato

# FIXME: Also generate intermediate images (gradients, etc.).

lena-s.ppm: $(top_srcdir)/milena/img/lena.pgm image2d$(EXEEXT)
	./image2d$(EXEEXT) $< 1000 $@

graph-s.png: graph-s.neato
	neato -Tpng $< >$@
graph-s.neato: seeds.pgm graph$(EXEEXT)
	./graph$(EXEEXT) $< 2 $@

m283-s.off: m283-c.off complex$(EXEEXT)
	./complex$(EXEEXT) $< 100 $@

classwst-graph.ppm: classseedsi.pbm classif-graph$(EXEEXT)
	./classif-graph$(EXEEXT) $< 4 $@

classwst-1complex.ppm: classseedsi.pbm classif-1complex$(EXEEXT)
	./classif-1complex$(EXEEXT) $< 4 $@
