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

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  var						\
  min						\
  min_h						\
  min_max					\
  max						\
  max_h						\
  median_h					\
  mean						\
  rank

var_SOURCES = var.cc
mean_SOURCES = mean.cc
median_h_SOURCES = median_h.cc
min_SOURCES = min.cc
min_h_SOURCES = min_h.cc
max_SOURCES = max.cc
max_h_SOURCES = max_h.cc
min_max_SOURCES = min_max.cc
rank_SOURCES = rank.cc

TESTS = $(check_PROGRAMS)

