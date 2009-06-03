## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  dim2

check_PROGRAMS =				\
  compute					\
  immerse					\
  is_pixel					\
  is_separator					\
  separator_to_pixels

compute_SOURCES = compute.cc
immerse_SOURCES = immerse.cc
is_pixel_SOURCES = is_pixel.cc
is_separator_SOURCES = is_separator.cc
separator_to_pixels_SOURCES = separator_to_pixels.cc

TESTS = $(check_PROGRAMS)
