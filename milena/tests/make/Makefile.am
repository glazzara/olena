## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  dual_neighb					\
  image2d					\
  image3d					\
  mat						\
  w_window					\
  w_window_directional

dual_neighb_SOURCES = dual_neighb.cc
image2d_SOURCES = image2d.cc
image3d_SOURCES = image3d.cc
mat_SOURCES = mat.cc
w_window_SOURCES = w_window.cc
w_window_directional_SOURCES = w_window_directional.cc

TESTS = $(check_PROGRAMS)
