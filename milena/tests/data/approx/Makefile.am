# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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
  median

# FIXME: The current median test is way too long to run.  Break it
# into two tests: a fast one, executed during `make check', compiled
# with no optimization, using a small kernel and a small image; and a
# second one, run during `make check-full', using a big kernel on a
# normal image (e.g. Lena) and full optimization (`-O3').
#
# For the moment, we just have the second test.  We run it during
# target `check', but we shall move it to target `check-full' as soon
# as a smaller and faster test is written.
median_CXXFLAGS = $(LONG_TESTS_CXXFLAGS)

TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES =				\
  median-out_oct.pgm				\
  median-out_rec.pgm
