## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  contrast					\
  dilation					\
  dilation_max_h				\
  erosion					\
  erosion_min_h					\
  gradient					\
  hit_or_miss					\
  laplacian					\
  opening_area					\
  thinning

contrast_SOURCES = contrast.cc
dilation_SOURCES = dilation.cc
dilation_max_h_SOURCES = dilation_max_h.cc
erosion_SOURCES = erosion.cc
erosion_min_h_SOURCES = erosion_min_h.cc
gradient_SOURCES = gradient.cc
hit_or_miss_SOURCES = hit_or_miss.cc
laplacian_SOURCES = laplacian.cc
opening_area_SOURCES = opening_area.cc
thinning_SOURCES = thinning.cc

TESTS = $(check_PROGRAMS)
