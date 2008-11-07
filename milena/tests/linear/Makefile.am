## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = local

check_PROGRAMS =				\
  convolve					\
  convolve_directional				\
  convolve_2x1d					\
  gaussian					\
  lap						\
  log						\
  sobel_2d

convolve_SOURCES = convolve.cc
gaussian_SOURCES = gaussian.cc
lap_SOURCES = lap.cc
convolve_directional_SOURCES = convolve_directional.cc
convolve_2x1d_SOURCES = convolve_2x1d.cc
log_SOURCES = log.cc
sobel_2d_SOURCES = sobel_2d.cc

TESTS = $(check_PROGRAMS)
