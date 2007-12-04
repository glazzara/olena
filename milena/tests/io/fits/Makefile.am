## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =

if CFITSIO
  check_PROGRAMS += fits
  fits_SOURCES = fits.cc
  fits_CPPFLAGS = $(AM_CPPFLAGS) $(CFITSIO_CPPFLAGS)
  fits_LDFLAGS = $(AM_LDFLAGS) $(CFITSIO_LDFLAGS)

endif

TESTS = $(check_PROGRAMS)
