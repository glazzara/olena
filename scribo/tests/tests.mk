## tests.mk					-*- Makefile-Automake -*-

## Shared configuration for tests.

## Copyright (C) 2009, 2010, 2013 EPITA Research and Development
## Laboratory (LRDE).
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.

AM_CPPFLAGS = 			\
	-I$(top_srcdir)/scribo 	\
	-I$(top_builddir)/scribo\
	-I$(top_srcdir)/milena 	\
	-I$(top_builddir)/milena

SCRIBO_TESTS_CXXFLAGS = @SCRIBO_TESTS_CXXFLAGS@
AM_CXXFLAGS = $(SCRIBO_TESTS_CXXFLAGS)

# Flags for optional dependencies.

MAGICKXX_CPPFLAGS = @MAGICKXX_CPPFLAGS@
MAGICKXX_LDFLAGS = @MAGICKXX_LDFLAGS@

TIFF_CPPFLAGS = @TIFF_CPPFLAGS@
TIFF_LDFLAGS = @TIFF_LDFLAGS@

QT_CPPFLAGS = @QT_CPPFLAGS@
QT_LDFLAGS  = @QT_LDFLAGS@ @QT_LIBS@

TESSERACT_CPPFLAGS = @TESSERACT_CPPFLAGS@
TESSERACT_LDFLAGS = @TESSERACT_LDFLAGS@


# Inject rules to compile tests (recursively), without running them.
include $(top_srcdir)/build-aux/tests-recursive.mk
