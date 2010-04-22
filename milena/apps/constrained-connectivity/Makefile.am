# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

noinst_PROGRAMS = constrained-connectivity
constrained_connectivity_CXXFLAGS = $(AM_CXXFLAGS) $(STRICT_ALIASING_CXXFLAGS)

TESTS = test-constrained-connectivity

# The sample image form Pierre Soille's PAMI 2008 article.
EXTRA_DIST = soille.pgm
