## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = make math

check_PROGRAMS =				\
  converts_to					\
  is						\
  unconst

converts_to_SOURCES = converts_to.cc
is_SOURCES = is.cc
unconst_SOURCES = unconst.cc

TESTS = $(check_PROGRAMS)
