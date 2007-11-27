## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  l1						\
  l2						\
  linfty

l1_SOURCES = l1.cc common.hh
l2_SOURCES = l2.cc common.hh
linfty_SOURCES = linfty.cc common.hh

TESTS = $(check_PROGRAMS)
