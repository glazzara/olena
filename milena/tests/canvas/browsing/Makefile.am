## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =					\
  fwd							\
  snake_fwd						\
  snake_vert						\
  hyper_directional					\
  dir_struct_elt_incr_update

fwd_SOURCES = fwd.cc
snake_fwd_SOURCES = snake_fwd.cc
snake_vert_SOURCES = snake_vert.cc
hyper_directional_SOURCES = hyper_directional.cc
dir_struct_elt_incr_update_SOURCES = dir_struct_elt_incr_update.cc

TESTS = $(check_PROGRAMS)
