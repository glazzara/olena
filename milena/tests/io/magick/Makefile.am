## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

# FIXME: We should not use a hard-coded value.  Rely on a variable set
# by configure instead.
AM_LDFLAGS = -lMagick++ 

check_PROGRAMS =				\
   load 					\
   save

load_SOURCES = load.cc
save_SOURCES = save.cc

TESTS = $(check_PROGRAMS)
