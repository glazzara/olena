# Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE).
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

include $(top_srcdir)/milena/common.mk

# Find apps/data.hh (generated header).
AM_CPPFLAGS += -I$(top_builddir)/milena
# Produce fast code with no debugging symbols.
AM_CXXFLAGS = $(APPS_CXXFLAGS_NODEBUG)

EXTRA_DIST = lena1024.pgm lena2048.pgm

noinst_PROGRAMS =				\
  dilation-lena					\
  dilation-lena-bench-nongen			\
  dilation-lena-bench-nongen_2ptr		\
  dilation-lena-bench-gen			\
  dilation-lena-bench-fast			\
  dilation-lena-bench-fast_static		\
  dilation-lena-table				\
  gradient-lena					\
  gradient-spe-lena

noinst_HEADERS =				\
  and_not.hh					\
  dilation-lena.hh				\
  dilation-lena-bench-macros.hh			\
  minus.hh					\
  static_array.hh				\
  static_dpoints_pixter.hh			\
  static_window.hh				\
  trait.hh

MOSTLYCLEANFILES =				\
  dilation-lena-out-512-nongen.pgm		\
  dilation-lena-out-512-nongen_2ptr.pgm		\
  dilation-lena-out-512-nongen_1ptr.pgm		\
  dilation-lena-out-512-gen.pgm			\
  dilation-lena-out-512-fast.pgm		\
  dilation-lena-out-512-fast_noaccu.pgm		\
  dilation-lena-out-512-faster.pgm		\
  dilation-lena-out-512-faster_noaccu.pgm	\
  dilation-lena-out-512-fast_static.pgm		\
  dilation-lena-out-512-faster_static.pgm	\
  dilation-lena-out-1024-nongen.pgm		\
  dilation-lena-out-1024-nongen_2ptr.pgm	\
  dilation-lena-out-1024-nongen_1ptr.pgm	\
  dilation-lena-out-1024-gen.pgm		\
  dilation-lena-out-1024-fast.pgm		\
  dilation-lena-out-1024-fast_noaccu.pgm	\
  dilation-lena-out-1024-faster.pgm		\
  dilation-lena-out-1024-faster_noaccu.pgm	\
  dilation-lena-out-1024-fast_static.pgm	\
  dilation-lena-out-1024-faster_static.pgm	\
  dilation-lena-out-2048-nongen.pgm		\
  dilation-lena-out-2048-nongen_2ptr.pgm	\
  dilation-lena-out-2048-nongen_1ptr.pgm	\
  dilation-lena-out-2048-gen.pgm		\
  dilation-lena-out-2048-fast.pgm		\
  dilation-lena-out-2048-fast_noaccu.pgm	\
  dilation-lena-out-2048-faster.pgm		\
  dilation-lena-out-2048-faster_noaccu.pgm	\
  dilation-lena-out-2048-fast_static.pgm	\
  dilation-lena-out-2048-faster_static.pgm	\
						\
  dilation-lena-table-out-512-nongen.pgm	\
  dilation-lena-table-out-512-nongen_2ptr.pgm	\
  dilation-lena-table-out-512-gen.pgm		\
  dilation-lena-table-out-512-fast.pgm		\
  dilation-lena-table-out-512-fast_static.pgm	\
  dilation-lena-table-out-1024-nongen.pgm	\
  dilation-lena-table-out-1024-nongen_2ptr.pgm	\
  dilation-lena-table-out-1024-gen.pgm		\
  dilation-lena-table-out-1024-fast.pgm		\
  dilation-lena-table-out-1024-fast_static.pgm	\
  dilation-lena-table-out-2048-nongen.pgm	\
  dilation-lena-table-out-2048-nongen_2ptr.pgm	\
  dilation-lena-table-out-2048-gen.pgm		\
  dilation-lena-table-out-2048-fast.pgm		\
  dilation-lena-table-out-2048-fast_static.pgm	\
						\
  gradient-lena-out.pgm				\
  gradient-spe-lena-out-0.pgm			\
  gradient-spe-lena-out-1.pgm			\
  gradient-spe-lena-out-2.pgm			\
  gradient-spe-lena-out-3.pgm			\
  gradient-spe-lena-out-4.pgm

# Use Make to generate tests instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed -e 's|@top_srcdir[@]|$(top_srcdir)|g'	\
           -e 's|@srcdir[@]|$(srcdir)|g'
EXTRA_DIST += test-dilation-lena-bench.in
test-dilation-lena-bench: test-dilation-lena-bench.in Makefile
	rm -f $@ $@.tmp
	srcdir='';				\
	  test -f ./$< || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$< >>$@.tmp
	chmod a-w,a+x $@.tmp
	mv $@.tmp $@
CLEANFILES = test-dilation-lena-bench

TESTS = test-dilation-lena-bench
