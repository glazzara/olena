## common.mk					-*- Makefile-Automake -*-

## Common Automake parameters.

## Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
## (LRDE).
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

# C++ sources.
AM_DEFAULT_SOURCE_EXT = .cc

# Find Scribo and Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/scribo -I$(top_srcdir)/milena
