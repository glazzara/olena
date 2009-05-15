## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  all_headers					\
  line						\
  graph

all_headers_SOURCES = all_headers.cc
line_SOURCES = line.cc
graph_SOURCES = graph.cc

TESTS = $(check_PROGRAMS)
