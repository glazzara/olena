## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  load_bin					\
  load_save_bin					\
						\
  load_float

load_bin_SOURCES = load_bin.cc
load_save_bin_SOURCES = load_save_bin.cc

load_float_SOURCES = load_float.cc

TESTS = $(check_PROGRAMS)
