## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  background					\
  blobs						\
  flat_zones					\
  foreground					\
  level						\
  regional_maxima				\
  regional_minima

background_SOURCES = background.cc
blobs_SOURCES = blobs.cc
flat_zones_SOURCES = flat_zones.cc
foreground_SOURCES = foreground.cc
level_SOURCES = level.cc
regional_maxima_SOURCES = regional_maxima.cc
regional_minima_SOURCES = regional_minima.cc

TESTS = $(check_PROGRAMS)
