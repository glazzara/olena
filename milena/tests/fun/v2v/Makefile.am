## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
	norm					\
	projection				\
	rgb_hsi_conversion

norm_SOURCES = norm.cc
projection_SOURCES = projection.cc
rgb_hsi_conversion_SOURCES = rgb_hsi_conversion.cc

TESTS = $(check_PROGRAMS)
