# Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE).
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
#

## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

noinst_PROGRAMS =


if HAVE_MAGICKXX

noinst_PROGRAMS += content_in_hdoc_hdlac

content_in_hdoc_hdlac_SOURCES = content_in_hdoc_hdlac.cc
content_in_hdoc_hdlac_CPPFLAGS = $(AM_CPPFLAGS) \
				 $(MAGICKXX_CPPFLAGS) \
				 $(TESSERACT_CPPFLAGS) \
				 -I$(top_builddir)
content_in_hdoc_hdlac_LDFLAGS  = $(AM_LDFLAGS) \
				 -lpthread \
				 $(MAGICKXX_LDFLAGS)

endif HAVE_MAGICKXX
