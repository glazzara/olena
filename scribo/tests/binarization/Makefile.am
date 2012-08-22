# Copyright (C) 2009, 2011, 2012 EPITA Research and Development
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

include $(top_srcdir)/scribo/tests/tests.mk

EXTRA_DIST = 					\
  kim.res.pbm					\
  kim_wodd_heven.ref.pbm			\
  kim_weven_hodd.ref.pbm			\
  kim_wodd_hodd.ref.pbm				\
  niblack.res.pbm				\
  niblack_wodd_heven.ref.pbm			\
  niblack_weven_hodd.ref.pbm			\
  niblack_wodd_hodd.ref.pbm			\
  niblack_fast.res.pbm				\
  niblack_fast_wodd_heven.ref.pbm		\
  niblack_fast_weven_hodd.ref.pbm		\
  niblack_fast_wodd_hodd.ref.pbm		\
  sauvola_ms.ref.pbm				\
  sauvola_ms_wodd_heven.ref.pbm			\
  sauvola_ms_weven_hodd.ref.pbm			\
  sauvola_ms_wodd_hodd.ref.pbm			\
  sauvola.ref.pbm				\
  sauvola_wodd_heven.ref.pbm			\
  sauvola_weven_hodd.ref.pbm			\
  sauvola_wodd_hodd.ref.pbm			\
  otsu.ref.pbm					\
  wolf.ref.pbm					\
  wolf_wodd_heven.ref.pbm			\
  wolf_weven_hodd.ref.pbm			\
  wolf_wodd_hodd.ref.pbm			\
  wolf_fast.ref.pbm				\
  wolf_fast_wodd_heven.ref.pbm			\
  wolf_fast_weven_hodd.ref.pbm			\
  wolf_fast_wodd_hodd.ref.pbm

check_PROGRAMS =				\
  global_threshold 				\
  kim 						\
  local_threshold	 			\
  niblack					\
  niblack_fast					\
  otsu						\
  sauvola 					\
  sauvola_ms					\
  wolf						\
  wolf_fast


global_threshold_SOURCES = global_threshold.cc
kim_SOURCES = kim.cc
local_threshold_SOURCES = local_threshold.cc
niblack_SOURCES = niblack.cc
niblack_fast_SOURCES = niblack_fast.cc
otsu_SOURCES = otsu.cc
sauvola_SOURCES = sauvola.cc
sauvola_ms_SOURCES = sauvola_ms.cc
wolf_SOURCES = wolf.cc
wolf_fast_SOURCES = wolf_fast.cc

TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES = res.pbm
