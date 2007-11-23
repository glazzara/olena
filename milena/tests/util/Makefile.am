## Process this file through Automake to create Makefile.in -*- Makefile -*-
check_PROGRAMS =				\
branch_iter					\
branch_iter_ind					\
graph

branch_iter_SOURCES = branch_iter.cc
branch_iter_ind_SOURCES = branch_iter_ind.cc
graph_SOURCES = graph.cc

TESTS = $(check_PROGRAMS)

include $(top_srcdir)/milena/tests/tests.mk
