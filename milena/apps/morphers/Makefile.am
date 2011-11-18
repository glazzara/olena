# Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE).
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

# Illustrations of morphers.

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

noinst_PROGRAMS =				\
  mask+channel					\
  recorder					\
  mask+recorder

mask_channel_SOURCES = mask+channel.cc
recorder_SOURCES = recorder.cc recorder.hh
mask_recorder_SOURCES = mask+recorder.cc recorder.hh

MOSTLYCLEANFILES =				\
  lena-mask-channel.ppm				\
  lena-fill??????.ppm				\
  lena-roi-fill??????.ppm

# FIXME: Also produce movies (see comments in recorder.cc and
# mask+recorder.cc.)
