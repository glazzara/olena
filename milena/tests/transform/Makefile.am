# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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
  bench_closest_point_geodesic			\
  distance_and_closest_point_geodesic		\
  distance_front				\
  distance_geodesic				\
  hough 					\
  kht 						\
  influence_zone_front				\
  influence_zone_geodesic 			\
  influence_zone_geodesic_saturated

bench_closest_point_geodesic_SOURCES = bench_closest_point_geodesic.cc
distance_and_closest_point_geodesic_SOURCES = distance_and_closest_point_geodesic.cc
distance_front_SOURCES = distance_front.cc
distance_geodesic_SOURCES = distance_geodesic.cc
hough_SOURCES = hough.cc
kht_SOURCES = kht.cc
influence_zone_front_SOURCES = influence_zone_front.cc
influence_zone_geodesic_SOURCES = influence_zone_geodesic.cc
influence_zone_geodesic_saturated_SOURCES = influence_zone_geodesic_saturated.cc

TESTS = $(check_PROGRAMS)
