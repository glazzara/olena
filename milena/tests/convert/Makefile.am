## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

##FIXME: re-enable all tests
check_PROGRAMS =				\
  to_hsl 					\
  to_image					\
  to_p_array					\
  to_p_set					\
  to_rgb 					\
  to_window

to_hsl_SOURCES = to_hsl.cc
to_image_SOURCES = to_image.cc
to_p_array_SOURCES = to_p_array.cc
to_p_set_SOURCES = to_p_set.cc
to_rgb_SOURCES = to_rgb.cc
to_window_SOURCES = to_window.cc

TESTS = $(check_PROGRAMS)
