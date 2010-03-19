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

SUBDIRS =					\
  binarization 					\
  debug 					\
  primitive 					\
  filter 					\
  misc 						\
  multi_scale 					\
  preprocessing 				\
  table 					\
  text

bin_PROGRAMS = 				\
  pbm_lines_in_doc 			\
  text_in_photo 			\
  text_in_photo_ppm 			\
  text_in_photo_ppm_fast		\
  text_in_photo_pbm_fast		\
  text_in_photo_invert


pbm_lines_in_doc_SOURCES = pbm_lines_in_doc.cc
text_in_photo_SOURCES = text_in_photo.cc
text_in_photo_ppm_SOURCES = text_in_photo_ppm.cc
text_in_photo_ppm_fast_SOURCES = text_in_photo_ppm_fast.cc
text_in_photo_pbm_fast_SOURCES = text_in_photo_pbm_fast.cc
text_in_photo_invert_SOURCES = text_in_photo_invert.cc



if HAVE_TESSERACT
if HAVE_TIFF

    bin_PROGRAMS 	+= text_in_doc
    text_in_doc_SOURCES = text_in_doc.cc
    text_in_doc_CPPFLAGS = $(AM_CPPFLAGS) \
			   $(TESSERACT_CPPFLAGS) \
			   $(TIFF_CPPFLAGS)
    text_in_doc_LDFLAGS  = $(AM_LDFLAGS) \
			   $(TESSERACT_LDFLAGS) \
			   $(TIFF_LDFLAGS) \
			   -lpthread -lhpdf

    bin_PROGRAMS 	+= text_in_doc_ppm
    text_in_doc_ppm_SOURCES = text_in_doc_ppm.cc
    text_in_doc_ppm_CPPFLAGS = $(AM_CPPFLAGS) \
			   $(TESSERACT_CPPFLAGS) \
			   $(TIFF_CPPFLAGS)
    text_in_doc_ppm_LDFLAGS  = $(AM_LDFLAGS) \
			   $(TESSERACT_LDFLAGS) \
			   $(TIFF_LDFLAGS) \
			   -lpthread -lhpdf


    bin_PROGRAMS	+= text_in_article_pbm
    text_in_article_pbm_CPPFLAGS = $(AM_CPPFLAGS) \
			       $(TESSERACT_CPPFLAGS) \
			       $(TIFF_CPPFLAGS)
    text_in_article_pbm_LDFLAGS  = $(AM_LDFLAGS) \
			       $(TESSERACT_LDFLAGS) \
			       $(TIFF_LDFLAGS) \
			       -lpthread
# -lhpdf
    text_in_article_pbm_SOURCES = text_in_article_pbm.cc


    text_in_photo_ppm_fast_CPPFLAGS = $(AM_CPPFLAGS) \
			       		$(TESSERACT_CPPFLAGS) \
			       		$(TIFF_CPPFLAGS)
    text_in_photo_ppm_fast_LDFLAGS  = $(AM_LDFLAGS) \
			     	  	$(TESSERACT_LDFLAGS) \
			       		$(TIFF_LDFLAGS) \
			      	 	-lpthread

endif HAVE_TIFF
endif HAVE_TESSERACT
