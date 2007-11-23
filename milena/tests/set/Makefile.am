## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  diff						\
  inter						\
  is_subset_of					\
  sym_diff					\
  uni

diff_SOURCES = diff.cc
inter_SOURCES = inter.cc
is_subset_of_SOURCES = is_subset_of.cc
sym_diff_SOURCES = sym_diff.cc
uni_SOURCES = uni.cc

TESTS = $(check_PROGRAMS)
