## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  all_headers					\
  and						\
  and_not					\
  not						\
  or						\
  xor

all_headers_SOURCES = all_headers.cc
and_SOURCES = and.cc
and_not_SOURCES = and_not.cc
not_SOURCES = not.cc
or_SOURCES = or.cc
xor_SOURCES = xor.cc

TESTS = $(check_PROGRAMS)
