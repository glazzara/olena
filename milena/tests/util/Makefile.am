## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk


check_PROGRAMS =				\
  branch_iter					\
  branch_iter_ind				\
  line_graph 					\
  eat						\
  graph						\
  lazy_set					\
  lemmings					\
  ord_pair					\
  soft_heap 					\
  tree						\
  tree_fast					\
  tree_fast_to_image				\
  tree_to_image					\
  tree_to_fast

branch_iter_SOURCES = branch_iter.cc
branch_iter_ind_SOURCES = branch_iter_ind.cc
line_graph_SOURCES = line_graph.cc
eat_SOURCES = eat.cc
graph_SOURCES = graph.cc
lazy_set_SOURCES = lazy_set.cc
lemmings_SOURCES = lemmings.cc
ord_pair_SOURCES = ord_pair.cc
soft_heap_SOURCES = soft_heap.cc
tree_SOURCES = tree.cc
tree_fast_SOURCES = tree_fast.cc
tree_fast_to_image_SOURCES = tree_to_image.cc
tree_to_image_SOURCES = tree_to_image.cc
tree_to_fast_SOURCES = tree_to_fast.cc

TESTS = $(check_PROGRAMS)
