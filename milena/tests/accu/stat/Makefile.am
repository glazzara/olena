## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  var						\
  min						\
  min_h						\
  min_max					\
  max						\
  max_h						\
  median_h					\
  mean

var_SOURCES = var.cc
mean_SOURCES = min.cc
median_h_SOURCES = min_h.cc
min_SOURCES = min.cc
min_h_SOURCES = min_h.cc
max_SOURCES = max.cc
max_h_SOURCES = max_h.cc
min_max_SOURCES = min_max.cc

TESTS = $(check_PROGRAMS)
