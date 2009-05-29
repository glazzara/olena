## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
	hsl_to_rgb 	 			\
	norm					\
	projection				\
	rgb_to_hsl 				\
	wrap

norm_SOURCES = norm.cc
projection_SOURCES = projection.cc
rgb_to_hsl_SOURCES = rgb_to_hsl.cc
hsl_to_rgb_SOURCES = hsl_to_rgb.cc
wrap_SOURCES = wrap.cc

TESTS = $(check_PROGRAMS)

XFAIL_TESTS = hsl_to_rgb
