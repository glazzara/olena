## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  adjust					\
  all_headers.cc				\
  duplicate					\
  equalize					\
  fill						\
  find						\
  get						\
  mirror					\
  resize

adjust_SOURCES = adjust.cc
all_headers_SOURCES = all_headers.cc
duplicate_SOURCES = duplicate.cc
equalize_SOURCES = equalize.cc
fill_SOURCES = fill.cc
find_SOURCES = find.cc
get_SOURCES = get.cc
mirror_SOURCES = mirror.cc
resize_SOURCES = resize.cc

TESTS = $(check_PROGRAMS)
