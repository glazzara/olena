# Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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
#

## Process this file through Automake to create Makefile.in.

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

convolve_SOURCES = convolve.cc
lap_SOURCES = lap.cc
convolve_directional_SOURCES = convolve_directional.cc
convolve_2x1d_SOURCES = convolve_2x1d.cc
log_SOURCES = log.cc
sobel_2d_SOURCES = sobel_2d.cc

#<<lrde
# FIXME: Not distributed (yet).
check_PROGRAMS += gaussian_
gaussian__SOURCES = gaussian.cc
#>>

TESTS = $(check_PROGRAMS)
