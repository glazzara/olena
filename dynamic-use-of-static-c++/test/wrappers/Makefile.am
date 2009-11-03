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

LDADD =						\
  $(top_builddir)/src/wrappers/libdynmilena.la	\
  $(top_builddir)/src/libdyn.la 
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src

check_PROGRAMS = test-milena
test_milena_SOURCES = test-milena.cc

TESTS = $(check_PROGRAMS)
