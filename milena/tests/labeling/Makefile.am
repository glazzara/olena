## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  background					\
  blobs						\
  colorize 					\
  compute 					\
  fill_holes					\
  flat_zones					\
  foreground					\
  level						\
  mean_values 					\
  n_max						\
  pack 						\
  regional_maxima				\
  regional_minima 				\
  relabel 					\
  wrap

background_SOURCES = background.cc
blobs_SOURCES = blobs.cc
colorize_SOURCES = colorize.cc
compute_SOURCES = compute.cc
fill_holes_SOURCES = fill_holes.cc
flat_zones_SOURCES = flat_zones.cc
foreground_SOURCES = foreground.cc
level_SOURCES = level.cc
mean_values_SOURCES = mean_values.cc
n_max_SOURCES = n_max.cc
pack_SOURCES = pack.cc
regional_maxima_SOURCES = regional_maxima.cc
regional_minima_SOURCES = regional_minima.cc
relabel_SOURCES = relabel.cc
wrap_SOURCES = wrap.cc

TESTS = $(check_PROGRAMS)
