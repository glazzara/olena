## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

AM_CPPFLAGS += $(TIFF_CPPFLAGS)
AM_LDFLAGS = $(TIFF_LDFLAGS)

check_PROGRAMS = load

load_SOURCES = load.cc

TESTS = $(check_PROGRAMS)
