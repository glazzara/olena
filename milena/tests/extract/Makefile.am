# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
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

#<<lrde
# FIXME: Not distributed (yet).
check_PROGRAMS =				\
  blue						\
  green						\
  hue						\
  lum						\
  red						\
  sat

blue_SOURCES = blue.cc
green_SOURCES = green.cc
hue_SOURCES = hue.cc
lum_SOURCES = lum.cc
red_SOURCES = red.cc
sat_SOURCES = sat.cc
#>>

TESTS = $(check_PROGRAMS)
