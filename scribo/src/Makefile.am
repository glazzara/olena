# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

include $(top_srcdir)/scribo/scribo.mk

SUBDIRS =					\
  binarization 					\
  contest 					\
  debug 					\
  primitive 					\
  filter 					\
  misc 						\
  postprocessing 				\
  preprocessing 				\
  text

# Disabled for the moment:
#  table


bin_PROGRAMS =


if HAVE_TESSERACT

    bin_PROGRAMS	+= pbm_text_in_doc
    pbm_text_in_doc_CPPFLAGS = $(AM_CPPFLAGS) -g -ggdb \
			       $(TESSERACT_CPPFLAGS) \
			       $(TIFF_CPPFLAGS)
    pbm_text_in_doc_LDFLAGS  = $(AM_LDFLAGS) -g -ggdb\
			       $(TESSERACT_LDFLAGS) \
			       $(TIFF_LDFLAGS) \
			       -lpthread

    pbm_text_in_doc_SOURCES = pbm_text_in_doc.cc

endif HAVE_TESSERACT


if HAVE_MAGICKXX

    bin_PROGRAMS	+= text_in_doc_preprocess
    text_in_doc_preprocess_CPPFLAGS = $(AM_CPPFLAGS) \
				      $(MAGICKXX_CPPFLAGS)
    text_in_doc_preprocess_LDFLAGS  = $(AM_LDFLAGS) \
			      	      -lpthread \
				      $(MAGICKXX_LDFLAGS)

    text_in_doc_preprocess_SOURCES = text_in_doc_preprocess.cc



    bin_PROGRAMS	+= text_in_picture
    text_in_picture_SOURCES = text_in_picture.cc
    text_in_picture_CPPFLAGS = $(AM_CPPFLAGS) \
			       $(MAGICKXX_CPPFLAGS)
    text_in_picture_LDFLAGS  = $(AM_LDFLAGS) \
		      	       -lpthread \
			       $(MAGICKXX_LDFLAGS)

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
			   -lpthread

    bin_PROGRAMS 	+= text_in_doc_ppm
    text_in_doc_ppm_SOURCES = text_in_doc_ppm.cc
    text_in_doc_ppm_CPPFLAGS = $(AM_CPPFLAGS) \
			   $(TESSERACT_CPPFLAGS) \
			   $(TIFF_CPPFLAGS)
    text_in_doc_ppm_LDFLAGS  = $(AM_LDFLAGS) \
			   $(TESSERACT_LDFLAGS) \
			   $(TIFF_LDFLAGS) \
			   -lpthread

endif HAVE_TIFF

endif HAVE_TESSERACT

endif HAVE_MAGICKXX
