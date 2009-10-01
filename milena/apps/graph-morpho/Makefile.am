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

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

noinst_HEADERS = convert.hh debug.hh io.hh morpho.hh make_complex2d.hh

noinst_PROGRAMS = samples-complex1d samples-image2d asf-complex1d asf-image2d
samples_complex1d_SOURCES = samples-complex1d.cc
samples_image2d_SOURCES = samples-image2d.cc
asf_complex1d_SOURCES = asf-complex1d.cc
asf_image2d_SOURCES = asf-image2d.cc

TESTS = samples-complex1d samples-image2d

# Graph images stored in 2D PBM files.
EXTRA_DIST = x.pbm y.pbm z.pbm

# Actual PBM images.
EXTRA_DIST += zebra.pbm zebra-noisy.pbm
