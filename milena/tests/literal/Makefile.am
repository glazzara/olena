# Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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

check_PROGRAMS =				\
  black						\
  medium_gray					\
  one						\
  white						\
  zero

black_SOURCES = black.cc
medium_gray_SOURCES = medium_gray.cc
one_SOURCES = one.cc
white_SOURCES = white.cc
zero_SOURCES = zero.cc

TESTS = $(check_PROGRAMS)
