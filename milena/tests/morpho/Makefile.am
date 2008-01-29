## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  closing_area					\
  contrast					\
  dilation					\
  dilation_max_h				\
  erosion					\
  erosion_min_h					\
  extrema_components				\
  gradient					\
  hit_or_miss					\
  laplacian					\
  level_components				\
  opening_area					\
  thinning

closing_area_SOURCES = closing_area.cc
contrast_SOURCES = contrast.cc
dilation_SOURCES = dilation.cc
dilation_max_h_SOURCES = dilation_max_h.cc
erosion_SOURCES = erosion.cc
erosion_min_h_SOURCES = erosion_min_h.cc
extrema_components_SOURCES = extrema_components.cc
gradient_SOURCES = gradient.cc
hit_or_miss_SOURCES = hit_or_miss.cc
laplacian_SOURCES = laplacian.cc
level_components_SOURCES = level_components.cc
opening_area_SOURCES = opening_area.cc
thinning_SOURCES = thinning.cc

TESTS = $(check_PROGRAMS)
