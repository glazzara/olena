# Copyright (C) 2010, 2011, 2013 EPITA Research and Development Laboratory
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

include $(top_srcdir)/scribo/tests/tests.mk

check_PROGRAMS =



if HAVE_QT
if HAVE_TESSERACT
if HAVE_MAGICKXX

check_PROGRAMS += text_extraction
text_extraction_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
text_extraction_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) \
			   $(TESSERACT_CPPFLAGS) \
			   $(TIFF_CPPFLAGS) \
			   $(MAGICKXX_CPPFLAGS) \
			   -I$(top_builddir)
text_extraction_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS) \
			   $(TESSERACT_LDFLAGS) \
			   $(TIFF_LDFLAGS) \
			   $(MAGICKXX_LDFLAGS) \
			   -lpthread
text_extraction_LDADD    = $(QT_LIBS) $(LDADD)

endif HAVE_MAGICKXX
endif HAVE_TESSERACT
endif HAVE_QT


TESTS = $(check_PROGRAMS)
