## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  distance					\
  distance_geodesic				\
  influence_zone_geodesic

distance_SOURCES = distance.cc
distance_geodesic_SOURCES = distance_geodesic.cc
influence_zone_geodesic_SOURCES = influence_zone_geodesic.cc

TESTS = $(check_PROGRAMS)
