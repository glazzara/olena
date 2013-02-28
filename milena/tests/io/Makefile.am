# Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
# Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

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
  ppms						\
  fld

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

if HAVE_POPPLER
  SUBDIRS += pdf
endif HAVE_POPPLER