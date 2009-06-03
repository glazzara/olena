## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

AM_CPPFLAGS += $(MAGICKXX_CPPFLAGS)
AM_LDFLAGS = $(MAGICKXX_LDFLAGS)

check_PROGRAMS =				\
   load 					\
   save

load_SOURCES = load.cc
save_SOURCES = save.cc

TESTS = $(check_PROGRAMS)
XFAIL_TESTS = save
