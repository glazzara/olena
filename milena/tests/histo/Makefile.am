## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  compute					\
  point_from_value				\
  to_image1d

compute_SOURCES = compute.cc
point_from_value_SOURCES = point_from_value.cc
to_image1d_SOURCES = to_image1d.cc

TESTS = $(check_PROGRAMS)
