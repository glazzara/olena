#								-*- Automake -*-

# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
# (LRDE).
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

AM_CPPFLAGS += -I$(top_srcdir)/milena/doc/tutorial/tools

## FIXME: Upercase variables should be reserved for special names:
## classical Make variables (e.g. `CXXFLAGS') Automake variables
## (e.g. `SUBDIRS'), etc.
##
## Moreover, do we really need all these variables?  See how things
## evolve during the refurbishing of the build system in doc/.
DOC_SRCDIR = $(top_srcdir)/milena/doc
OUTPUTS_SRCDIR = $(DOC_SRCDIR)/outputs
SPLIT_OUTPUTS_SRCDIR = $(OUTPUTS_SRCDIR)/split
FIGURES_SRCDIR = $(DOC_SRCDIR)/figures
EXAMPLES_SRCDIR = $(DOC_SRCDIR)/examples
SPLIT_EXAMPLES_SRCDIR = $(EXAMPLES_SRCDIR)/split
IMG_SRCDIR = $(DOC_SRCDIR)/img
