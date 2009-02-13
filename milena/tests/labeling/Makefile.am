## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  background					\
  blobs						\
  compute 					\
  fill_holes					\
  flat_zones					\
  foreground					\
  level						\
  regional_maxima				\
  regional_minima 				\
  relabel

background_SOURCES = background.cc
blobs_SOURCES = blobs.cc
compute_SOURCES = compute.cc
fill_holes_SOURCES = fill_holes.cc
flat_zones_SOURCES = flat_zones.cc
foreground_SOURCES = foreground.cc
level_SOURCES = level.cc
regional_maxima_SOURCES = regional_maxima.cc
regional_minima_SOURCES = regional_minima.cc
relabel_SOURCES = relabel.cc

TESTS = $(check_PROGRAMS)
