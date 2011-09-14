# Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
# (LRDE).
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

include $(top_srcdir)/scribo/tests/tests.mk

EXTRA_DIST = 					\
  sauvola_ms.ref.pbm				\
  sauvola.ref.pbm

check_PROGRAMS =				\
  global_threshold 				\
  local_threshold	 			\
  sauvola 					\
  sauvola_ms


global_threshold_SOURCES = global_threshold.cc
local_threshold_SOURCES = local_threshold.cc
sauvola_SOURCES = sauvola.cc
sauvola_ms_SOURCES = sauvola_ms.cc

TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES = res.pbm
