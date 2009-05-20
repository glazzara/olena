## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  operators					\
  p_array					\
##  p_bgraph					\
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

operators_SOURCES = operators.cc
p_array_SOURCES = p_array.cc
##p_bgraph_SOURCES = p_bgraph.cc
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
