## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  elementary					\
  tree

check_PROGRAMS =				\
  artificial_line_graph_image_wst		\
  closing_area					\
  closing_height				\
  closing_volume				\
  combined					\
  complex_image_morpho				\
  complex_image_wst				\
  contrast					\
  dilation					\
  erosion					\
  general					\
  gradient					\
  graph_image_morpho				\
  graph_image_wst				\
  hit_or_miss					\
  laplacian					\
  lena_line_graph_image_wst1			\
  lena_line_graph_image_wst2			\
  line_graph_image_wst				\
  line_graph_image_morpho			\
  meyer_wst					\
  meyer_wst_long				\
  opening_area					\
  opening_height				\
  opening_volume				\
  rank_filter 					\
  skeleton_constrained				\
  thinning

# -------------- #
# Normal tests.  #
# -------------- #

# FIXME: Have erosion and dilation perform symmetric tests.
erosion_SOURCES = erosion.cc

opening_area_SOURCES = opening_area.cc
closing_area_SOURCES = closing_area.cc
closing_height_SOURCES = closing_height.cc
opening_height_SOURCES = opening_height.cc
closing_volume_SOURCES = closing_volume.cc
opening_volume_SOURCES = opening_volume.cc

contrast_SOURCES = contrast.cc
general_SOURCES = general.cc
gradient_SOURCES = gradient.cc
hit_or_miss_SOURCES = hit_or_miss.cc
laplacian_SOURCES = laplacian.cc
thinning_SOURCES = thinning.cc

graph_image_morpho_SOURCES = graph_image_morpho.cc
graph_image_wst_SOURCES = graph_image_wst.cc

line_graph_image_morpho_SOURCES = line_graph_image_morpho.cc
line_graph_image_wst_SOURCES = line_graph_image_wst.cc

complex_image_morpho_SOURCES = complex_image_morpho.cc
complex_image_wst_SOURCES = complex_image_wst.cc

meyer_wst_SOURCES = meyer_wst.cc

rank_filter_SOURCES = rank_filter.cc

skeleton_constrained_SOURCES = skeleton_constrained.cc

combined_SOURCES = combined.cc

# --------------- #
# Complex tests.  #
# --------------- #

artificial_line_graph_image_wst_SOURCES = artificial_line_graph_image_wst.cc
artificial_line_graph_image_wst_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

dilation_SOURCES = dilation.cc
dilation_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

lena_line_graph_image_wst1_SOURCES = lena_line_graph_image_wst1.cc
lena_line_graph_image_wst1_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

lena_line_graph_image_wst2_SOURCES = lena_line_graph_image_wst2.cc
lena_line_graph_image_wst2_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

meyer_wst_long_SOURCES = meyer_wst_long.cc
meyer_wst_long_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)


TESTS = $(check_PROGRAMS)
