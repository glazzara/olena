## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  p_array					\
  p_bgraph					\
  p_image2d					\
  p_priority_queue				\
  p_priority_queue_fast				\
  p_priority_queue_fast_with_array		\
  p_queue					\
  p_queue_fast					\
  p_runs					\
  p_set						\
  pset_array					\
  pset_if

p_array_SOURCES = p_array.cc
p_bgraph_SOURCES = p_bgraph.cc
p_image2d_SOURCES = p_image2d.cc
p_priority_queue_SOURCES = p_priority_queue.cc
p_priority_queue_fast_SOURCES = p_priority_queue_fast.cc
p_priority_queue_fast_with_array_SOURCES = p_priority_queue_fast.cc
p_queue_SOURCES = p_priority_queue_fast.cc
p_queue_fast_SOURCES = p_priority_queue_fast.cc
p_runs_SOURCES = p_runs.cc
p_set_SOURCES = p_set.cc
pset_array_SOURCES = pset_array.cc
pset_if_SOURCES = pset_if.cc

TESTS = $(check_PROGRAMS)
