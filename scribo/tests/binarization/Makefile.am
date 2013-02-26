# Copyright (C) 2009, 2011, 2013 EPITA Research and Development Laboratory
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

include $(top_srcdir)/scribo/tests/tests.mk

EXTRA_DIST = 					\
  niblack.res.pbm				\
  sauvola_ms.ref.pbm				\
  sauvola.ref.pbm				\
  otsu.ref.pbm

check_PROGRAMS =				\
  global_threshold 				\
  local_threshold	 			\
  niblack					\
  otsu						\
  sauvola 					\
  sauvola_ms


TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES = res.pbm
