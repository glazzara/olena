## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  p_array					\
##  p_bgraph					\
  p_edges 					\
  p_image					\
  p_priority_queue				\
  p_queue					\
  p_queue_fast					\
  p_set						\
  pset_if 					\
  p_vertices

p_array_SOURCES = p_array.cc
##p_bgraph_SOURCES = p_bgraph.cc
p_image_SOURCES = p_image.cc
p_priority_queue_SOURCES = p_priority_queue.cc
p_queue_SOURCES = p_queue.cc
p_queue_fast_SOURCES = p_queue_fast.cc
p_set_SOURCES = p_set.cc
pset_if_SOURCES = pset_if.cc
p_vertices_SOURCES = p_vertices.cc
p_edges_SOURCES = p_edges.cc

TESTS = $(check_PROGRAMS)
