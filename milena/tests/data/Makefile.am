# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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
  saturate					\
  sort_psites					\
  stretch					\
  transform					\
  transform_inplace				\
  update

abs_SOURCES = abs.cc
apply_SOURCES = apply.cc
compare_SOURCES = compare.cc
compute_SOURCES = compute.cc
convert_SOURCES = convert.cc
fill_SOURCES = fill.cc
fill_with_image_SOURCES = fill_with_image.cc
fill_with_value_SOURCES = fill_with_value.cc
median_SOURCES = median.cc
median_fast_SOURCES = median_fast.cc
memcpy__SOURCES = memcpy_.cc
memset__SOURCES = memset_.cc
paste_SOURCES = paste.cc
saturate_SOURCES = saturate.cc
sort_psites_SOURCES = sort_psites.cc
stretch_SOURCES = stretch.cc
transform_SOURCES = transform.cc
transform_inplace_SOURCES = transform_inplace.cc
update_SOURCES = update.cc

# Lengthy tests.
# FIXME: enable it when make full-check is enabled.
#fill_full_SOURCES = fill_full.cc

TESTS = $(check_PROGRAMS)

