## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  card						\
  compute					\
  compute_with_weights				\
  diff						\
  inter						\
  sym_diff					\
  uni						\
  unique

card_SOURCES = card.cc
compute_SOURCES = compute.cc
compute_with_weights_SOURCES = compute_with_weights.cc
diff_SOURCES = diff.cc
inter_SOURCES = inter.cc
sym_diff_SOURCES = sym_diff.cc
uni_SOURCES = uni.cc
unique_SOURCES = unique.cc

TESTS = $(check_PROGRAMS)
