## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

AM_CPPFLAGS += $(CFITSIO_CPPFLAGS)
AM_LDFLAGS = $(CFITSIO_LDFLAGS)

check_PROGRAMS = fits

fits_SOURCES = fits.cc

TESTS = $(check_PROGRAMS)
