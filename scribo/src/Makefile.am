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
  extract 					\
  filter 					\
  preprocessing 				\
  table 					\
  text

bin_PROGRAMS = 				\
  dmap                               	\
  morpho                    		\
  negate 				\
  preprocess 				\
  rectangularity 			\
  superpose 				\
  table_rebuild_opening 		\
  table_rebuild_rank 			\
  text_in_photo 			\
  text_in_photo_invert 			\
  thin_bboxes


dmap_SOURCES = dmap.cc
morpho_SOURCES = morpho.cc
negate_SOURCES = negate.cc
preprocess_SOURCES = preprocess.cc
rectangularity_SOURCES = rectangularity.cc
superpose_SOURCES = superpose.cc
table_rebuild_opening_SOURCES = table_rebuild_opening.cc
table_rebuild_rank_SOURCES = table_rebuild_rank.cc
text_in_photo_SOURCES = text_in_photo.cc
text_in_photo_invert_SOURCES = text_in_photo_invert.cc
thin_bboxes_SOURCES = thin_bboxes.cc



if HAVE_TESSERACT
if HAVE_TIFF

    bin_PROGRAMS 	+= recognition
    recognition_SOURCES  = recognition.cc
    recognition_CPPFLAGS = $(AM_CPPFLAGS) \
			   $(TESSERACT_CPPFLAGS) \
			   $(TIFF_CPPFLAGS)
    recognition_LDFLAGS  = $(AM_LDFLAGS) \
			   $(TESSERACT_LDFLAGS) \
			   $(TIFF_LDFLAGS) \
			   -lpthread

    bin_PROGRAMS 	+= text_in_doc
    text_in_doc_SOURCES = text_in_doc.cc
    text_in_doc_CPPFLAGS = $(AM_CPPFLAGS) \
			   $(TESSERACT_CPPFLAGS) \
			   $(TIFF_CPPFLAGS)
    text_in_doc_LDFLAGS  = $(AM_LDFLAGS) \
			   $(TESSERACT_LDFLAGS) \
			   $(TIFF_LDFLAGS) \
			   -lpthread

endif HAVE_TIFF
endif HAVE_TESSERACT
