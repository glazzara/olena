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

include $(srcdir)/tests.mk

EXTRA_DIST =				\
  img/alignment_1.pbm			\
  img/alignment_2.pbm			\
  img/alignment_3.pbm			\
  img/alignment_4.pbm			\
  img/comp_on_borders.pbm		\
  img/multi_scale.png			\
  img/phillip.pbm			\
  img/phillip.ppm			\
  img/pixels.pbm			\
  img/several_objects.pgm		\
  img/single_object.pbm			\
  img/table_to_be_repaired.pbm		\
  img/table_to_be_repaired2.pbm		\
  img/table_to_be_repaired3.pbm		\
  img/text_to_group.pbm			\
  img/text_to_group.pgm			\
  img/text_to_group_and_clean.pbm	\
  img/the_valleys.pbm			\
  img/wildly.pbm

SUBDIRS =		\
  binarization		\
  convert		\
  core			\
  estim			\
  filter		\
  preprocessing		\
  primitive		\
  table			\
  text			\
  toolchain		\
  unit_test		\
  util

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += unit_test
