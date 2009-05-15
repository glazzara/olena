## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  all_headers					\
  diff_abs					\
  minus						\
  plus						\
  revert					\
  times

all_headers_SOURCES = all_headers.cc
diff_abs_SOURCES = diff_abs.cc
minus_SOURCES = minus.cc
plus_SOURCES = plus.cc
revert_SOURCES = revert.cc
times_SOURCES = times.cc

TESTS = $(check_PROGRAMS)
