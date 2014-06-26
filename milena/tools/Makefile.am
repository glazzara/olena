# Copyright (C) 2008, 2009, 2010, 2011, 2013, 2014 EPITA Research and
# Development Laboratory (LRDE).
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

# Produce fast code.
AM_CXXFLAGS = $(TOOLS_CXXFLAGS)

# FIXME: Add some tests for these tools.
bin_PROGRAMS =					\
  seed2tiling 					\
  area_flooding

# FIXME: Rename this program.
noinst_PROGRAMS = compute_local_configurations

noinst_HEADERS = connectivity_numbers_3d.hh
noinst_PROGRAMS += connectivity_numbers_3d


# FIXME: Hard-coded paths.
CPPFLAGS_TBB = -I/opt/local/include
LDFLAGS_TBB = -L/opt/local/lib -ltbb

.PHONY: extra
extra: $(EXTRA_PROGRAMS)

noinst_HEADERS += connectivity_numbers_3d_tbb.hh
EXTRA_PROGRAMS = connectivity_numbers_3d_tbb
connectivity_numbers_3d_tbb_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_TBB)
connectivity_numbers_3d_tbb_LDFLAGS =  $(LDFLAGS_TBB)

noinst_HEADERS += simple_point3d_lut.hh
EXTRA_PROGRAMS += simple_point3d_lut

noinst_HEADERS += simple_point3d_lut_tbb.hh
EXTRA_PROGRAMS += simple_point3d_lut_tbb
simple_point3d_lut_tbb_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_TBB)
simple_point3d_lut_tbb_LDFLAGS =  $(LDFLAGS_TBB)

# Simple point look-up tables (LUTs).
.PHONY: simple_point_luts
simple_point_luts: simple_point3d_lut_26_6.hh

CLEANFILES = simple_point3d_lut_26_6.txt
simple_point3d_lut_26_6.txt: simple_point3d_lut_tbb$(EXEEXT)
	./simple_point3d_lut_tbb$(EXEEXT) 26_6 >$@

EXTRA_DIST = gen_cxx_lut
CLEANFILES += simple_point3d_lut_26_6.hh
simple_point3d_lut_26_6.hh: simple_point3d_lut_26_6.txt $(srcdir)/gen_cxx_lut
	rm -f $@.tmp
	$(srcdir)/gen_cxx_lut simple_point3d_lut_26_6 "(26, 6)" \
	  <simple_point3d_lut_26_6.txt >$@.tmp
	mv $@.tmp $@

# Very long (both to compile and execute) optional test, checking the
# consistency of the LUT in simple_point3d_lut_26_6.hh.  Moreover, it
# should be turned into a parallel program.

.PHONY: check-extra
check-extra: test_simple_point3d_lut_26_6$(EXEEXT)
	./test_simple_point3d_lut_26_6$(EXEEXT)

EXTRA_LTLIBRARIES = libsimple_point3d_lut_26_6.la
libsimple_point3d_lut_26_6_la_SOURCES = simple_point3d_lut_26_6.cc
nodist_libsimple_point3d_lut_26_6_la_SOURCES = simple_point3d_lut_26_6.hh
# Generate this build dependency of libsimple_point3d_lut_26_6.la.
simple_point3d_lut_26_6.lo: simple_point3d_lut_26_6.hh

EXTRA_PROGRAMS += test_simple_point3d_lut_26_6
test_simple_point3d_lut_26_6_LDADD = libsimple_point3d_lut_26_6.la
# Generate this build dependency of test_simple_point3d_lut_26_6.
test_simple_point3d_lut_26_6.o: simple_point3d_lut_26_6.hh

CLEANFILES += $(EXTRA_PROGRAMS) $(EXTRA_LTLIBRARIES)
