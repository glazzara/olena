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

## FIXME: Revamp (see Olena 0.11's test suite).

# Look for Milena header in srcdir.
AM_CPPFLAGS = -I$(top_srcdir)/milena

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 (or just a part of it) is available in the distribution,
# instead of using the hand-made TOOLS_CXXFLAGS.
TOOLS_CXXFLAGS = @TOOLS_CXXFLAGS@
AM_CXXFLAGS = $(TOOLS_CXXFLAGS)
