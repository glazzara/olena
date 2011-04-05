# Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

include $(top_srcdir)/scribo/scribo.mk


noinst_PROGRAMS = 	\
  discontinued_lines  	\
  discontinued_vlines   \
  discontinued_hlines  	\
  separators_nonvisible \
  thick_vlines  	\
  thick_hlines  	\
  lines_pattern

discontinued_lines_SOURCES = discontinued_lines.cc
discontinued_vlines_SOURCES = discontinued_vlines.cc
discontinued_hlines_SOURCES = discontinued_hlines.cc
separators_nonvisible_SOURCES = separators_nonvisible.cc
thick_vlines_SOURCES = thick_vlines.cc
thick_hlines_SOURCES = thick_hlines.cc
lines_pattern_SOURCES = lines_pattern.cc


if HAVE_MAGICKXX

    noinst_PROGRAMS    += alignments
    alignments_SOURCES  = alignments.cc
    alignments_CPPFLAGS = $(AM_CPPFLAGS) \
			  $(MAGICKXX_CPPFLAGS)
    alignments_LDFLAGS  = $(AM_LDFLAGS) \
			  $(MAGICKXX_LDFLAGS)
endif HAVE_MAGICKXX