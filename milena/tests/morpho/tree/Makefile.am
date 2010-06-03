# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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

check_PROGRAMS =				\
  compute_attribute_image			\
  compute_parent				\
  dual_input_tree				\
  data	 					\
  max

compute_attribute_image_SOURCES = compute_attribute_image.cc
compute_parent_SOURCES = compute_parent.cc
dual_input_tree_SOURCES = dual_input_tree.cc
data_SOURCES = data.cc
max_SOURCES = max.cc

TESTS = $(check_PROGRAMS)

SUBDIRS = \
  filter
