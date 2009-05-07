## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  dim2

check_PROGRAMS =				\
  image2full					\
  immerse

image2full_SOURCES = image2full.cc
immerse_SOURCES = immerse.cc

TESTS = $(check_PROGRAMS)
