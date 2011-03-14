# Copyright (C) 2009, 2010, 2011 EPITA Research and Development
# Laboratory (LRDE).
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

noinst_PROGRAMS = 				\
  show_components_bboxes 			\
  show_groups_bboxes 				\
  show_info_x_height 				\
  show_info_median_inter_characters		\
  show_separators				\
  show_links_left_right_links_validation 	\
  show_links_bbox_h_ratio 			\
  show_links_bbox_overlap 			\
  show_links_bottom_aligned 			\
  show_links_bottom_aligned_2angles		\
  show_links_center_aligned 			\
  show_links_non_h_aligned 			\
  show_links_several_right_overlap		\
  show_links_single_down 			\
  show_links_single_down_left_aligned		\
  show_links_single_down_right_aligned		\
  show_links_single_left 			\
  show_links_single_left_dmax_ratio		\
  show_links_single_right 			\
  show_links_single_right_dmax_ratio		\
  show_links_single_up 	 			\
  show_links_single_up_left_aligned		\
  show_links_single_up_right_aligned		\
  show_links_top_aligned 			\
  show_links_top_aligned_2angles		\
  show_objects_large 				\
  show_objects_large_small			\
  show_objects_small 				\
  show_objects_thick 				\
  show_objects_thin


show_components_bboxes_SOURCES = show_components_bboxes.cc
show_groups_bboxes_SOURCES = show_groups_bboxes.cc
show_info_x_height_SOURCES = show_info_x_height.cc
show_info_median_inter_characters_SOURCES = show_info_median_inter_characters.cc
show_separators_SOURCES = show_separators.cc
show_links_left_right_links_validation_SOURCES = show_links_left_right_links_validation.cc
show_links_bbox_h_ratio_SOURCES = show_links_bbox_h_ratio.cc
show_links_bbox_overlap_SOURCES = show_links_bbox_overlap.cc
show_links_bottom_aligned_SOURCES = show_links_bottom_aligned.cc
show_links_bottom_aligned_2angles_SOURCES = show_links_bottom_aligned_2angles.cc
show_links_center_aligned_SOURCES = show_links_center_aligned.cc
show_links_non_h_aligned_SOURCES = show_links_non_h_aligned.cc
show_links_several_right_overlap_SOURCES = show_links_several_right_overlap.cc
show_links_single_down_SOURCES = show_links_single_down.cc
show_links_single_down_left_aligned_SOURCES = show_links_single_down_left_aligned.cc
show_links_single_down_right_aligned_SOURCES = show_links_single_down_right_aligned.cc
show_links_single_left_SOURCES = show_links_single_left.cc
show_links_single_left_dmax_ratio_SOURCES = show_links_single_left_dmax_ratio.cc
show_links_single_right_SOURCES = show_links_single_right.cc
show_links_single_right_dmax_ratio_SOURCES = show_links_single_right_dmax_ratio.cc
show_links_single_up_SOURCES = show_links_single_up.cc
show_links_single_up_left_aligned_SOURCES = show_links_single_up_left_aligned.cc
show_links_single_up_right_aligned_SOURCES = show_links_single_up_right_aligned.cc
show_links_top_aligned_SOURCES = show_links_top_aligned.cc
show_links_top_aligned_2angles_SOURCES = show_links_top_aligned_2angles.cc
show_objects_large_SOURCES = show_objects_large.cc
show_objects_large_small_SOURCES = show_objects_large_small.cc
show_objects_small_SOURCES = show_objects_small.cc
show_objects_thick_SOURCES = show_objects_thick.cc
show_objects_thin_SOURCES = show_objects_thin.cc

if HAVE_MAGICKXX

if HAVE_QT

  noinst_PROGRAMS +=  show_paragraph_blocks
  show_paragraph_blocks_SOURCES  = show_paragraph_blocks.cc
  show_paragraph_blocks_CPPFLAGS = $(AM_CPPFLAGS) \
				   $(QT_CPPFLAGS) \
				   $(MAGICKXX_CPPFLAGS)
  show_paragraph_blocks_CXXFLAGS = $(AM_CXXFLAGS) \
				   $(QT_CXXFLAGS)
  show_paragraph_blocks_LDFLAGS  = $(AM_LDFLAGS) \
				   $(QT_LDFLAGS) \
				   $(MAGICKXX_LDFLAGS)
  show_paragraph_blocks_LDADD    = $(LDADD) \
				   $(QT_LIBS)

  noinst_PROGRAMS  	  += show_text_lines
  show_text_lines_SOURCES  = show_text_lines.cc
  show_text_lines_CPPFLAGS = $(AM_CPPFLAGS) \
		  	     $(MAGICKXX_CPPFLAGS) \
			     $(QT_CPPFLAGS) -DHAVE_QT
  show_text_lines_CXXFLAGS = $(AM_CXXFLAGS) \
		  	     $(QT_CXXFLAGS)
  show_text_lines_LDFLAGS  = $(AM_LDFLAGS) \
			     $(MAGICKXX_LDFLAGS) \
			     $(QT_LDFLAGS)
  show_text_lines_LDADD    = $(LDADD) \
			     $(QT_LIBS)


  noinst_PROGRAMS       += show_stoppers
  show_stoppers_SOURCES  = show_stoppers.cc
  show_stoppers_CPPFLAGS = $(AM_CPPFLAGS) \
		  	   $(MAGICKXX_CPPFLAGS) \
			   $(QT_CPPFLAGS)
  show_stoppers_CXXFLAGS = $(AM_CXXFLAGS) \
		  	   $(QT_CXXFLAGS)
  show_stoppers_LDFLAGS  = $(AM_LDFLAGS) \
			   $(MAGICKXX_LDFLAGS) \
			   $(QT_LDFLAGS)
  show_stoppers_LDADD    = $(LDADD) \
			   $(QT_LIBS)


endif HAVE_QT


  noinst_PROGRAMS 	      += highlight_text_area
  highlight_text_area_SOURCES  = highlight_text_area.cc
  highlight_text_area_CPPFLAGS = $(AM_CPPFLAGS) \
				 $(MAGICKXX_CPPFLAGS)
  highlight_text_area_LDFLAGS  = $(AM_LDFLAGS) \
				 $(MAGICKXX_LDFLAGS)

endif HAVE_MAGICKXX
