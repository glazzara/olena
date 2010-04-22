# Copyright (C) 2007, 2008, 2009, 2010, 2012, 2013 EPITA Research and
# Development Laboratory (LRDE).
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
  builtin					\
  concept

# FIXME: Re-enable all the tests.
check_PROGRAMS =				\
  bool						\
  equiv						\
  graylevel					\
  int_s						\
  int_s16					\
  int_u8					\
  interop					\
  label						\
  proxy						\
  rgb8						\
  scalar					\
  set						\
  sign
#  float01				
#  float01_bis			
#  float01_f		
#  graylevel_f					


# FIXME: Enable when make check_full works.
# check_full_PROGRAMS =				\
#   graylevel_full				\
#   graylevel_f_full				\
#   rgb_full
#

TESTS = $(check_PROGRAMS)
# FIXME: Enable when make check_full works.
# TESTS_FULL = $(check_full_PROGRAMS)

# FIXME: What's the purpose of this file?
EXTRA_DIST = macros.hh
