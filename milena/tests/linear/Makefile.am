## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = local

check_PROGRAMS =				\
  convolve					\
  gaussian					\
  lap						\
  line_convolve					\
  log						\
  sobel

convolve_SOURCES = convolve.cc
gaussian_SOURCES = gaussian.cc
lap_SOURCES = lap.cc
line_convolve_SOURCES = line_convolve.cc
log_SOURCES = log.cc
sobel_SOURCES = sobel.cc

TESTS = $(check_PROGRAMS)
