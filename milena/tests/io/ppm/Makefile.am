# Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE).
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

check_PROGRAMS =				\
     ppm16					\
     ppm23					\
     ppm

ppm16_SOURCES = ppm16.cc
ppm23_SOURCES = ppm23.cc
ppm_SOURCES = ppm.cc

TESTS = $(check_PROGRAMS)



# FIXME: each test should clean its output, temporary output should
# not be cleaned up by make.
CLEANFILES =					\
  out.ppm					\
  out16.ppm					\
  out23.ppm					\
  out8.ppm
