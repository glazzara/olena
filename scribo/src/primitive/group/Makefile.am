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

include $(top_srcdir)/scribo/scribo.mk

bin_PROGRAMS = 					\
  group_from_double_link           		\
  group_from_double_link_filter_non_aligned 	\
  group_from_double_several_links  		\
  group_from_graph                 		\
  group_from_several_graph         		\
  group_from_several_left_links    		\
  group_from_single_link

group_from_double_link_SOURCES = group_from_double_link.cc
group_from_double_link_filter_non_aligned_SOURCES = group_from_double_link_filter_non_aligned.cc
group_from_double_several_links_SOURCES = group_from_double_several_links.cc
group_from_graph_SOURCES = group_from_graph.cc
group_from_several_graph_SOURCES = group_from_several_graph.cc
group_from_several_left_links_SOURCES = group_from_several_left_links.cc
group_from_single_link_SOURCES = group_from_single_link.cc

## FIXME: Does not compile.
## bin_PROGRAMS += group_from_rag
## group_from_rag_SOURCES = group_from_rag.cc
