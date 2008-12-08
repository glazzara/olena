## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  compute					\
  diff						\
  inter						\
  sym_diff					\
  uni

compute_SOURCES = compute.cc
diff_SOURCES = diff.cc
inter_SOURCES = inter.cc
sym_diff_SOURCES = sym_diff.cc
uni_SOURCES = uni.cc

TESTS = $(check_PROGRAMS)
