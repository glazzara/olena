# Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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
  image 					\
  site_set 					\
  stat						\
  math						\
  shape

check_PROGRAMS =				\
  all_accus					\
  compute					\
  histo						\
  line						\
  max_site 					\
  nil						\
  pair						\
  transform					\
  transform_diagonal				\
  transform_directional				\
  transform_line				\
  transform_snake

all_accus_SOURCES = all_accus.cc
compute_SOURCES = compute.cc
histo_SOURCES = histo.cc
line_SOURCES = line.cc
max_site_SOURCES = max_site.cc
nil_SOURCES = nil.cc
pair_SOURCES = pair.cc
transform_SOURCES = transform.cc
transform_diagonal_SOURCES = transform_diagonal.cc
transform_directional_SOURCES = transform_directional.cc
transform_line_SOURCES = transform_line.cc
transform_snake_SOURCES = transform_snake.cc

if HAVE_BOOST_TUPLE
  check_PROGRAMS += tuple
  tuple_SOURCES = tuple.cc
endif HAVE_BOOST_TUPLE

TESTS = $(check_PROGRAMS)
