## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk


check_PROGRAMS =				\
  adjacency_matrix 				\
  branch_iter					\
  branch_iter_ind				\
  eat						\
  fibonacci_heap 				\
  graph						\
  lazy_set					\
  lemmings					\
  line_graph 					\
  ord_pair					\
  set 						\
  soft_heap 					\
  tree						\
  tree_fast					\
  tree_fast_to_image				\
  tree_to_fast

adjacency_matrix_SOURCES = adjacency_matrix.cc
branch_iter_SOURCES = branch_iter.cc
branch_iter_ind_SOURCES = branch_iter_ind.cc
eat_SOURCES = eat.cc
fibonacci_heap_SOURCES = fibonacci_heap.cc
graph_SOURCES = graph.cc
lazy_set_SOURCES = lazy_set.cc
lemmings_SOURCES = lemmings.cc
line_graph_SOURCES = line_graph.cc
ord_pair_SOURCES = ord_pair.cc
set_SOURCES = set.cc
soft_heap_SOURCES = soft_heap.cc
tree_SOURCES = tree.cc
tree_fast_SOURCES = tree_fast.cc
tree_to_fast_SOURCES = tree_to_fast.cc

TESTS = $(check_PROGRAMS)

