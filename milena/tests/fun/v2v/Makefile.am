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

check_PROGRAMS =				\
	hsl_to_rgb 	 			\
	norm					\
	projection				\
	rgb_to_hsl 				\
	wrap

norm_SOURCES = norm.cc
projection_SOURCES = projection.cc
rgb_to_hsl_SOURCES = rgb_to_hsl.cc
hsl_to_rgb_SOURCES = hsl_to_rgb.cc
wrap_SOURCES = wrap.cc

TESTS = $(check_PROGRAMS)

XFAIL_TESTS = hsl_to_rgb
