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

# FIXME: Rename this program.
noinst_PROGRAMS = compute_local_configurations

noinst_HEADERS = connectivity_numbers_3d.hh
noinst_PROGRAMS += connectivity_numbers_3d
connectivity_numbers_3d_SOURCES = connectivity_numbers_3d.cc


# FIXME: Hard-coded paths.
CPPFLAGS_TBB = -I/opt/local/include
LDFLAGS_TBB = -L/opt/local/lib -ltbb

.PHONY: extra
extra: $(EXTRA_PROGRAMS)

noinst_HEADERS += connectivity_numbers_3d_tbb.hh
EXTRA_PROGRAMS = connectivity_numbers_3d_tbb
connectivity_numbers_3d_tbb_SOURCES = connectivity_numbers_3d_tbb.cc
connectivity_numbers_3d_tbb_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_TBB)
connectivity_numbers_3d_tbb_LDFLAGS =  $(LDFLAGS_TBB)

noinst_HEADERS += simple_point_3d_lut.hh
EXTRA_PROGRAMS += simple_point_3d_lut
simple_point_3d_lut_SOURCES = simple_point_3d_lut.cc

noinst_HEADERS += simple_point_3d_lut_tbb.hh
EXTRA_PROGRAMS += simple_point_3d_lut_tbb
simple_point_3d_lut_tbb_SOURCES = simple_point_3d_lut_tbb.cc
simple_point_3d_lut_tbb_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_TBB)
simple_point_3d_lut_tbb_LDFLAGS =  $(LDFLAGS_TBB)

# Simple point look-up tables (LUTs).
.PHONY: simple_point_luts
simple_point_luts: simple_point_3d_lut_26_6.hh

CLEANFILES = simple_point_3d_lut_26_6.txt
# Careful, there is no dependency on the generator (on purpose,
# because regenerating these files takes time).
simple_point_3d_lut_26_6.txt:
	./simple_point_3d_lut_tbb$(EXEEXT) 26_6 >$@

CLEANFILES += simple_point_3d_lut_26_6.hh
## FIXME: Add copyright headers, guards and documentation (and
## declaration/definition separation).  It may be a good idea to move
## the action of this target into own file (outside this Makefile).
# The sed scripts remove the last (empty) line as well as the last
# comma, and add some identation.
simple_point_3d_lut_26_6.hh: simple_point_3d_lut_26_6.txt
	rm -f $@.tmp
	{								      \
	  set -e;							      \
	  echo "    extern const unsigned char simple_point_3d_lut_26_6[] ="; \
	  echo "      {";						      \
	  sed -ne '$$!p' $< | sed -e 's/^/        /' -e '$$s/,$$//';	      \
	  echo "      };";						      \
	} >$@.tmp
	mv $@.tmp $@
