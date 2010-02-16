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
#

## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  box 						\
  operators					\
  p_array					\
  p_complex					\
  p_edges 					\
  p_image					\
  p_priority_queue				\
  p_queue					\
  p_queue_fast					\
  p_set						\
  p_if  					\
  p_transformed					\
  p_vaccess 					\
  p_vertices

box_SOURCES = box.cc
operators_SOURCES = operators.cc
p_array_SOURCES = p_array.cc
p_complex_SOURCES = p_complex.cc
p_image_SOURCES = p_image.cc
p_priority_queue_SOURCES = p_priority_queue.cc
p_queue_SOURCES = p_queue.cc
p_queue_fast_SOURCES = p_queue_fast.cc
p_set_SOURCES = p_set.cc
p_if_SOURCES = p_if.cc
p_transformed_SOURCES = p_transformed.cc
p_vaccess_SOURCES = p_vaccess.cc
p_vertices_SOURCES = p_vertices.cc
p_edges_SOURCES = p_edges.cc

TESTS = $(check_PROGRAMS)
