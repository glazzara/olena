# Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.
#

## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  approx					\
  attribute					\
  closing 					\
  elementary					\
  opening 					\
  reconstruction				\
  tree						\
  watershed

check_PROGRAMS =				\
  artificial_line_graph_image_wst		\
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
  rank_filter 					\
  Rd						\
  skeleton_constrained				\
  thinning					\
  top_hat


# -------------- #
# Normal tests.  #
# -------------- #

# FIXME: Have erosion and dilation perform symmetric tests.
erosion_SOURCES = erosion.cc
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
top_hat_SOURCES = top_hat.cc
Rd_SOURCES = Rd.cc


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


# FIXME: each test should clean its output, temporary output should
# not be cleaned up by make.
CLEANFILES =					\
  out.ppm					\
  out.pbm					\
  out3.pgm					\
  wst.neato					\
  out1.pgm					\
  out2.pgm					\
  out.pgm
