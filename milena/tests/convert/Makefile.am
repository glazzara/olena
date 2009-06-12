# Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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

SUBDIRS = impl

##FIXME: re-enable all tests
check_PROGRAMS =				\
  to_image					\
  to_p_array					\
  to_p_set					\
  to_rgb 					\
  to_window

# to_hsl_SOURCES = to_hsl.cc  De-activated for 1.0
to_image_SOURCES = to_image.cc
to_p_array_SOURCES = to_p_array.cc
to_p_set_SOURCES = to_p_set.cc
to_rgb_SOURCES = to_rgb.cc
to_window_SOURCES = to_window.cc

TESTS = $(check_PROGRAMS)
