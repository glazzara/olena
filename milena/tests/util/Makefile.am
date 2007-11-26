## Process this file through Automake to create Makefile.in -*- Makefile -*-

check_PROGRAMS =				\
branch_iter					\
branch_iter_ind					\
eat						\
graph						\
ordpair						\
tree						\
tree_fast					\
tree_fast_to_image				\
tree_to_image					\
tree_fast_to_image


branch_iter_SOURCES = branch_iter.cc
branch_iter_ind_SOURCES = branch_iter_ind.cc
eat_SOURCES = eat.cc
graph_SOURCES = graph.cc
ordpair_SOURCES = ordpair.cc
tree_SOURCES = tree.cc
tree_fast_SOURCES = tree_fast.cc
tree_fast_to_image_SOURCES = tree_to_image.cc
tree_to_image_SOURCES = tree_to_image.cc
tree_fast_to_image_SOURCES = tree_fast_to_image.cc

TESTS = $(check_PROGRAMS)

include $(top_srcdir)/milena/tests/tests.mk
