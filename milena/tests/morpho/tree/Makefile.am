## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  compute_attribute_image			\
  compute_parent				\
  data	 					\
  max

compute_attribute_image_SOURCES = compute_attribute_image.cc
compute_parent_SOURCES = compute_parent.cc
data_SOURCES = data.cc
max_SOURCES = max.cc

TESTS = $(check_PROGRAMS)
