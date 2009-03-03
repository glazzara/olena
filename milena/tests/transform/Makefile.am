## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  bench_closest_point_geodesic			\
  distance_and_closest_point_geodesic		\
  distance_front				\
  distance_geodesic				\
  influence_zone_front				\
  influence_zone_geodesic

bench_closest_point_geodesic_SOURCES = bench_closest_point_geodesic.cc
distance_and_closest_point_geodesic_SOURCES = distance_and_closest_point_geodesic.cc
distance_front_SOURCES = distance_front.cc
distance_geodesic_SOURCES = distance_geodesic.cc
influence_zone_front_SOURCES = influence_zone_front.cc
influence_zone_geodesic_SOURCES = influence_zone_geodesic.cc

TESTS = $(check_PROGRAMS)
