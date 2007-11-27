## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk


check_PROGRAMS =				\
  all_headers.cc				\
  branch_iter					\
  branch_iter_ind				\
  eat						\
  graph						\
  ordpair					\
  tree						\
  tree_fast					\
  tree_fast_to_image				\
  tree_to_image

all_headers_SOURCES = all_headers.cc
branch_iter_SOURCES = branch_iter.cc
branch_iter_ind_SOURCES = branch_iter_ind.cc
eat_SOURCES = eat.cc
graph_SOURCES = graph.cc
ordpair_SOURCES = ordpair.cc
tree_SOURCES = tree.cc
tree_fast_SOURCES = tree_fast.cc
tree_fast_to_image_SOURCES = tree_to_image.cc
tree_to_image_SOURCES = tree_to_image.cc

TESTS = $(check_PROGRAMS)
