# Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

check_PROGRAMS =				\
  background					\
  blobs						\
  blobs_and_compute				\
  colorize 					\
  compute 					\
  fill_holes					\
  flat_zones					\
  foreground					\
  mean_values 					\
  n_max						\
  pack 						\
  regional_maxima				\
  regional_minima 				\
  relabel 					\
  value						\
  wrap

background_SOURCES = background.cc
blobs_SOURCES = blobs.cc
blobs_and_compute_SOURCES = blobs_and_compute.cc
colorize_SOURCES = colorize.cc
compute_SOURCES = compute.cc
fill_holes_SOURCES = fill_holes.cc
flat_zones_SOURCES = flat_zones.cc
foreground_SOURCES = foreground.cc
mean_values_SOURCES = mean_values.cc
n_max_SOURCES = n_max.cc
pack_SOURCES = pack.cc
regional_maxima_SOURCES = regional_maxima.cc
regional_minima_SOURCES = regional_minima.cc
relabel_SOURCES = relabel.cc
value_SOURCES = value.cc
wrap_SOURCES = wrap.cc

TESTS = $(check_PROGRAMS)
