# Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
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

SUBDIRS = python

# Meta-wrappers (templates), not generating a module, but factoring
# common parts.
meta_wrappers =					\
  box.ixx					\
  box_piter.ixx					\
  ch_value.ixx					\
  concat.ixx					\
  concrete.ixx					\
  coord.ixx					\
  fill.ixx					\
  image2d.ixx					\
  int_u.ixx					\
  intp.ixx					\
  morpho.ixx					\
  pgm.ixx					\
  ppm.ixx					\
  println.ixx					\
  python-utils.ixx				\
  rgb.ixx

# Wrappers (generating actual modules).
wrappers =					\
  box2d.i					\
  box2d_piter.i					\
  config.i					\
  dpoint2d.i					\
  image2d_int.i					\
  image2d_int_u8.i				\
  image2d_rgb8.i				\
  int_u24.i					\
  int_u8.i					\
  neighb2d.i					\
  point2d.i					\
  rgb8.i					\
  window2d.i					\
						\
  dynamic_image2d.i

EXTRA_DIST = $(meta_wrappers) $(wrappers)

check_SCRIPTS = run
