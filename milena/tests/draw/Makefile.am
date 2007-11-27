## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  all_headers					\
  line						\
  mesh

all_headers_SOURCES = all_headers.cc
line_SOURCES = line.cc
mesh_SOURCES = mesh.cc

TESTS = $(check_PROGRAMS)
