## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  duplicate						\
  exact						\
  extend					\
  initialize					\
  primary

duplicate_SOURCES = duplicate.cc
exact_SOURCES = exact.cc
extend_SOURCES = extend.cc
initialize_SOURCES = initialize.cc
primary_SOURCES = primary.cc

TESTS = $(check_PROGRAMS)
