## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =					\
  fwd							\
  snake_fwd						\
  snake_vert						\
  hyper_directional					\
  dir_struct_elt_incr_update				\
  depth_first_search					\
  breadth_first_search

fwd_SOURCES = fwd.cc
snake_fwd_SOURCES = snake_fwd.cc
snake_vert_SOURCES = snake_vert.cc
hyper_directional_SOURCES = hyper_directional.cc
dir_struct_elt_incr_update_SOURCES = dir_struct_elt_incr_update.cc
depth_first_search_SOURCES = depth_first_search.cc
breadth_first_search_SOURCES = breadth_first_search.cc

TESTS = $(check_PROGRAMS)
