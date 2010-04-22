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

SUBDIRS =					\
  local

check_PROGRAMS =				\
  convolve					\
  convolve_directional				\
  convolve_2x1d					\
  lap						\
  log						\
  sobel_2d


check_PROGRAMS += gaussian_
gaussian__SOURCES = gaussian.cc

TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES =				\
  convolve_2x1d-out.pgm				\
  convolve-out.pgm				\
  convolve_directional-out.pgm			\
  gaussian-out.pgm				\
  lap-out.pgm					\
  log-out.pgm					\
  sobel_2d-out.pgm
