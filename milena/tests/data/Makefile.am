## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  fill						\
  fill_with_value				\
  fill_with_image				\
  memcpy_					\
  memset_					\
  paste

fill_SOURCES = fill.cc
fill_with_value_SOURCES = fill_with_value.cc
fill_with_image_SOURCES = fill_with_image.cc
memcpy__SOURCES = memcpy_.cc
memset__SOURCES = memset_.cc
paste_SOURCES = paste.cc

# Lengthy tests.
fill_full_SOURCES = fill_full.cc

TESTS = $(check_PROGRAMS)
