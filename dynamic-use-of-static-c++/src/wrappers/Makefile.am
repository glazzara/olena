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

# FIXME: These wrappers should be optional (and likewise for all
# milena-related code); have configure provide an `--enable-milena'
# option.

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src

# Wrapping a few Milena routines.
lib_LTLIBRARIES = libdynmilena.la
libdynmilena_la_SOURCES = milena.hh milena.cc
