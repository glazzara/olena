# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
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

bin_PROGRAMS = 					\
  cleantxt


cleantxt_SOURCES = cleantxt.cc


if HAVE_TESSERACT
if HAVE_TIFF

    bin_PROGRAMS 	 	  += pbm_lines_recognition
    pbm_lines_recognition_SOURCES  = pbm_lines_recognition.cc
    pbm_lines_recognition_CPPFLAGS = $(AM_CPPFLAGS) \
		  		     $(TESSERACT_CPPFLAGS) \
				     $(TIFF_CPPFLAGS)
    pbm_lines_recognition_LDFLAGS  = $(AM_LDFLAGS) \
			 	     $(TESSERACT_LDFLAGS) \
				     $(TIFF_LDFLAGS) \
				     -lpthread

    bin_PROGRAMS 	     += pbm_recognition
    pbm_recognition_SOURCES   = pbm_recognition.cc
    pbm_recognition_CPPFLAGS  = $(AM_CPPFLAGS) \
			        $(TESSERACT_CPPFLAGS) \
			   	$(TIFF_CPPFLAGS)
    pbm_recognition_LDFLAGS   = $(AM_LDFLAGS) \
			   	$(TESSERACT_LDFLAGS) \
			   	$(TIFF_LDFLAGS) \
			   	-lpthread
endif HAVE_TIFF
endif HAVE_TESSERACT
