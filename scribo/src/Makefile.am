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
  table

bin_PROGRAMS = 				\
  dmap                               	\
  extract_text_double_link           	\
  extract_text_double_several_links  	\
  extract_text_graph                 	\
  extract_text_several_graph         	\
  extract_text_several_left_links    	\
  extract_text_single_link  		\
  morpho                    		\
  negate 				\
  preprocess 				\
  rectangularity 			\
  superpose 				\
  table_rebuild_opening 		\
  table_rebuild_rank 			\
  text_in_photo 			\
  thin_bboxes


dmap_SOURCES = dmap.cc
extract_text_double_link_SOURCES = extract_text_double_link.cc
extract_text_double_several_links_SOURCES = extract_text_double_several_links.cc
extract_text_graph_SOURCES = extract_text_graph.cc
extract_text_several_graph_SOURCES = extract_text_several_graph.cc
extract_text_several_left_links_SOURCES = extract_text_several_left_links.cc
extract_text_single_link_SOURCES = extract_text_single_link.cc
morpho_SOURCES = morpho.cc
negate_SOURCES = negate.cc
preprocess_SOURCES = preprocess.cc
rectangularity_SOURCES = rectangularity.cc
superpose_SOURCES = superpose.cc
table_rebuild_opening_SOURCES = table_rebuild_opening.cc
table_rebuild_rank_SOURCES = table_rebuild_rank.cc
text_in_photo_SOURCES = text_in_photo.cc
thin_bboxes_SOURCES = thin_bboxes.cc

if HAVE_TESSERACT
  bin_PROGRAMS += recognition
  recognition_SOURCES = recognition.cc
  recognition_CPPFLAGS = $(AM_CPPFLAGS) $(TESSERACT_CPPFLAGS)
  recognition_LDFLAGS = $(AM_LDFLAGS) $(TESSERACT_LDFLAGS) -lpthread
endif HAVE_TESSERACT
