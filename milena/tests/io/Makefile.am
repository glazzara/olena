## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

## ---------------------- ##
## Built-in I/O support.  ##
## ---------------------- ##

SUBDIRS =					\
  dump						\
  off						\
  pbm						\
  pbms						\
  pfm						\
  pgm						\
  pgms						\
  pnm						\
  ppm 						\
  ppms

## ------------------------------------------------- ##
## I/O routines depending on a third-party library.  ##
## ------------------------------------------------- ##

if HAVE_GDCM
  SUBDIRS += dicom
endif HAVE_GDCM

if HAVE_CFITSIO
  SUBDIRS += fits
endif HAVE_CFITSIO

if HAVE_MAGICKXX
  SUBDIRS += magick
endif HAVE_MAGICKXX

if HAVE_TIFF
  SUBDIRS += tiff
endif HAVE_TIFF
