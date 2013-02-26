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

if HAVE_BOOST_TUPLE
  check_PROGRAMS += tuple
  tuple_CPPFLAGS = $(AM_CPPFLAGS) $(BOOST_CPPFLAGS)
endif HAVE_BOOST_TUPLE

TESTS = $(check_PROGRAMS)
