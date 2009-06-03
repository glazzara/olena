## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

## ---------------------- ##
## Built-in I/O support.  ##
## ---------------------- ##

SUBDIRS =					\
  dicom						\
  dump						\
  magick 					\
  off						\
  pbm						\
  pfm						\
  pgm						\
  pnm						\
  ppm 						\
  tiff

## ------------------------------------------------- ##
## I/O routines depending on a third-party library.  ##
## ------------------------------------------------- ##

if HAVE_CFITSIO
  SUBDIRS += fits
endif HAVE_CFITSIO
