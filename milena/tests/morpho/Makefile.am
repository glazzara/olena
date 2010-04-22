# Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
# Laboratory (LRDE).
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




# --------------- #
# Complex tests.  #
# --------------- #

artificial_line_graph_image_wst_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

dilation_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

lena_line_graph_image_wst1_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

lena_line_graph_image_wst2_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)

meyer_wst_long_CXXFLAGS = $(TESTS_CXXFLAGS_SPEED)


TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES =				\
  artificial_line_graph_image_wst-out.ppm	\
  complex_image_wst-wst.neato			\
  constrast-out.pgm				\
  dilation-out1.pgm				\
  dilation-out2.pgm				\
  dilation-out3.pgm				\
  dilation-out4.pgm				\
  dilation-out5.pgm				\
  general-out_oct_ref.pgm			\
  gradient-out.pgm				\
  hit_or_miss-out.pbm				\
  laplacian-out.pgm				\
  lena_line_graph_image_wst1-out.ppm		\
  lena_line_graph_image_wst2-out.ppm		\
  meyer_wst-out.pgm				\
  meyer_wst_long-out.ppm			\
  thinning-out.pbm				\
  thinning-out.pgm
