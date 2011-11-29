# Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  accu						\
  algebra					\
  arith						\
  binarization					\
  border					\
  canvas					\
  convert					\
  core						\
  data						\
  debug						\
  display					\
  draw						\
  estim						\
  extension					\
  fun						\
  geom						\
  graph 					\
  histo						\
  io						\
  labeling					\
  linear					\
  literal					\
  logical					\
  make						\
  math						\
  metal						\
  morpho					\
  norm						\
  opt						\
  pw						\
  set						\
  tag						\
  test						\
  topo						\
  trace						\
  trait						\
  transform 					\
  unit_test					\
  upscaling 					\
  util						\
  value						\
  win 						\
  world

#<<lrde
SUBDIRS +=					\
  extract					\
  registration 					\
  subsampling
#>>

check_PROGRAMS =				\
  all_headers 					\
  essential_headers 				\
  global_vars

all_headers_SOURCES = all_headers.cc
all_headers_CPPFLAGS = $(AM_CPPFLAGS) \
  $(CFITSIO_CPPFLAGS) $(GDCM_CPPFLAGS) $(MAGICKXX_CPPFLAGS) $(TIFF_CPPFLAGS)
all_headers_LDFLAGS = \
  $(CFITSIO_LDFLAGS) $(GDCM_LDFLAGS) $(MAGICKXX_LDFLAGS) $(TIFF_LDFLAGS)

essential_headers_SOURCES = essential_headers.cc

global_vars_SOURCES = global_vars1.cc global_vars2.cc
global_vars_CXXFLAGS = -DMLN_WO_GLOBAL_VARS

TESTS = $(check_PROGRAMS)

EXTRA_DIST = tools/pretty_check.sh
.PHONY: pretty-check
pretty-check:
	$(top_srcdir)/milena/tests/tools/pretty_check.sh

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += unit_test

EXTRA_DIST += 					\
  img/lena_ascii.pbm 				\
  img/lena_ascii.pgm				\
  img/lena_raw.pbm				\
  img/lena_raw.pgm				\
  img/test_bw.tif				\
  img/test_gl.tif				\
  img/test_rgb8.tif				\
  img/tiny.png

