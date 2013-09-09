# Copyright (C) 2009, 2010, 2013 EPITA Research and Development
# Laboratory (LRDE)
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

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena
# Find apps/data.hh (generated header).
AM_CPPFLAGS += -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)
STRICT_ALIASING_CXXFLAGS = @STRICT_ALIASING_CXXFLAGS@

noinst_HEADERS = convert.hh debug.hh make.hh morpho.hh make_complex2d.hh

noinst_PROGRAMS = samples-complex1d samples-image2d asf-complex1d asf-image2d
samples_image2d_CXXFLAGS = $(AM_CXXFLAGS) $(STRICT_ALIASING_CXXFLAGS)

TESTS = samples-complex1d samples-image2d

# Graph images stored in 2D PBM files.
EXTRA_DIST = x.pbm y.pbm z.pbm

# Actual PBM images.
EXTRA_DIST += zebra.pbm zebra-noisy.pbm
