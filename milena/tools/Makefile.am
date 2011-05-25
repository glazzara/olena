# Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
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

include $(top_srcdir)/milena/tools/tools.mk

# FIXME: Add some tests for these tools.
bin_PROGRAMS =					\
  seed2tiling 					\
  area_flooding

noinst_PROGRAMS = compute_local_configurations

noinst_PROGRAMS += connectivity_numbers_3d
connectivity_numbers_3d_SOURCES =		\
  connectivity_numbers_3d.hh			\
  connectivity_numbers_3d.cc


# FIXME: Hard-coded paths.
CPPFLAGS_TBB = -I/opt/local/include
LDFLAGS_TBB = -L/opt/local/lib -ltbb

.PHONY: extra
extra: $(EXTRA_PROGRAMS)

EXTRA_PROGRAMS = connectivity_numbers_3d_tbb
connectivity_numbers_3d_tbb_SOURCES =		\
  connectivity_numbers_3d.hh			\
  connectivity_numbers_3d_tbb.hh		\
  connectivity_numbers_3d_tbb.cc
connectivity_numbers_3d_tbb_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_TBB)
connectivity_numbers_3d_tbb_LDFLAGS =  $(LDFLAGS_TBB)
