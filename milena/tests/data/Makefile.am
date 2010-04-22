# Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = approx naive

check_PROGRAMS =				\
  abs						\
  apply						\
  compare					\
  compute					\
  convert					\
  fill						\
  fill_with_image				\
  fill_with_value				\
  median					\
  median_fast					\
  memcpy_					\
  memset_					\
  paste						\
  paste_without_localization 			\
  saturate					\
  sort_psites					\
  split 					\
  stretch					\
  transform					\
  transform_inplace				\
  update


# Lengthy tests.
# FIXME: Enable `fill_full' it when make full-check is enabled.


TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES =				\
  median-out_line.pgm				\
  median-out_rect.pgm				\
  median_fast-out.pgm
