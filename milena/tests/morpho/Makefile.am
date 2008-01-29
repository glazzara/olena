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
  meyer_wst					\
  meyer_wst_long				\
  opening_area					\
  thinning

dilation_SOURCES = dilation.cc
erosion_SOURCES = erosion.cc

dilation_max_h_SOURCES = dilation_max_h.cc
erosion_min_h_SOURCES = erosion_min_h.cc

opening_area_SOURCES = opening_area.cc
closing_area_SOURCES = closing_area.cc

level_components_SOURCES = level_components.cc
extrema_components_SOURCES = extrema_components.cc

contrast_SOURCES = contrast.cc
gradient_SOURCES = gradient.cc
hit_or_miss_SOURCES = hit_or_miss.cc
laplacian_SOURCES = laplacian.cc
thinning_SOURCES = thinning.cc

meyer_wst_SOURCES = meyer_wst.cc
# FIXME: We should isolate this test, as it takes a long time.
meyer_wst_long_SOURCES = meyer_wst_long.cc
meyer_wst_long_CXXFLAGS = -O3

TESTS = $(check_PROGRAMS)
