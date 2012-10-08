# Copyright (C) 2007, 2008, 2009, 2010, 2012 EPITA Research and Development
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
include $(top_srcdir)/build-aux/autotroll.mk

SUBDIRS = impl

check_PROGRAMS =				\
  to_image					\
  to_p_array					\
  to_p_set					\
  to_rgb 					\
  to_window

to_image_SOURCES = to_image.cc
to_p_array_SOURCES = to_p_array.cc
to_p_set_SOURCES = to_p_set.cc
to_rgb_SOURCES = to_rgb.cc
to_window_SOURCES = to_window.cc

if HAVE_QT

check_PROGRAMS += to_qimage
to_qimage_SOURCES = to_qimage.cc
to_qimage_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir)
to_qimage_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -O3
to_qimage_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
to_qimage_LDADD    = $(QT_LIBS) $(LDADD)

check_PROGRAMS += to_qimage_nocopy
to_qimage_nocopy_SOURCES = to_qimage_nocopy.cc
to_qimage_nocopy_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir)
to_qimage_nocopy_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -O3
to_qimage_nocopy_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
to_qimage_nocopy_LDADD    = $(QT_LIBS) $(LDADD)


endif HAVE_QT


TESTS = $(check_PROGRAMS)
