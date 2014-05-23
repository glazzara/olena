#! /usr/bin/env python

# Copyright (C) 2010, 2012, 2014 EPITA Research and Development
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

# \file python/max-tree.py
# \brief Test on the max-tree.

from swilena import *

import itertools

# Module aliases.
image = image2d_int_u8
par_image = image2d_point2d

ima = image.image2d_int_u8(3, 3)

values = [5, 6, 6,
          8, 9, 9,
          8, 9, 9]

for p, v in itertools.izip(ima.domain(), values):
  ima.set(p, int_u8(v))
image.println("ima =", ima)

max_tree_parent = image.max_tree(ima, c4())

par_image.println("max_tree_parent =", max_tree_parent)
