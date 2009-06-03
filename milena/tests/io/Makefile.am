## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

## ---------------------- ##
## Built-in I/O support.  ##
## ---------------------- ##

SUBDIRS =					\
  dicom						\
  dump						\
  off						\
  pbm						\
  pfm						\
  pgm						\
  pnm						\
  ppm

## ------------------------------------------------- ##
## I/O routines depending on a third-party library.  ##
## ------------------------------------------------- ##

if HAVE_CFITSIO
  SUBDIRS += fits
endif HAVE_CFITSIO

if HAVE_MAGICKXX
  SUBDIRS += magick
endif HAVE_MAGICKXX

if HAVE_TIFF
  SUBDIRS += tiff
endif HAVE_TIFF
