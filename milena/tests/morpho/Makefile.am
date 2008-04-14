## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  closing_area					\
  contrast					\
  dilation					\
  dilation_max_h				\
  erosion					\
  erosion_min_h					\
  gradient					\
  hit_or_miss					\
  laplacian					\
  lena_line_graph_image_wst1			\
  lena_line_graph_image_wst2			\
  meyer_wst					\
  meyer_wst_long				\
  opening_area					\
  thinning

# -------------- #
# Normal tests.  #
# -------------- #

# FIXME: Have erosion and dilation perform symmetric tests.
erosion_SOURCES = erosion.cc

dilation_max_h_SOURCES = dilation_max_h.cc
erosion_min_h_SOURCES = erosion_min_h.cc

opening_area_SOURCES = opening_area.cc
closing_area_SOURCES = closing_area.cc

contrast_SOURCES = contrast.cc
gradient_SOURCES = gradient.cc
hit_or_miss_SOURCES = hit_or_miss.cc
laplacian_SOURCES = laplacian.cc
thinning_SOURCES = thinning.cc

meyer_wst_SOURCES = meyer_wst.cc

# --------------- #
# Complex tests.  #
# --------------- #

dilation_SOURCES = dilation.cc
dilation_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

lena_line_graph_image_wst1_SOURCES = lena_line_graph_image_wst1.cc
lena_line_graph_image_wst1_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

lena_line_graph_image_wst2_SOURCES = lena_line_graph_image_wst2.cc
lena_line_graph_image_wst2_CXXFLAGS = $(TESTSCXXFLAGS_SPEED)

meyer_wst_long_SOURCES = meyer_wst_long.cc
meyer_wst_long_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)


TESTS = $(check_PROGRAMS)
