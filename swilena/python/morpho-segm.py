#! /usr/bin/env python

# Copyright (C) 2008, 2009, 2010, 2013 EPITA Research and Development
# Laboratory (LRDE)
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

# \file python/morpho-segm.py
# \brief Test on watershed transform-based segmentation.

import data
from swilena import *

# Module alias.
image = image2d_int_u8

ima = image.load(data.lena)

# Gradient.
gradient = image.gradient(ima, win_c4p())
image.save(gradient, "morpho-segm-gradient.pgm")
# Area closing of the gradient.
closed_gradient = image.closing_area(ima, c4(), 50)
# Watershed transform.
nbasins = int_u8()
ws = image.watershed_flooding(closed_gradient, c4(), nbasins)
print nbasins
image.save(ws, "morpho-segm-ws.pgm")

# FIXME: Also re-enable the naive segmentation with no gradient
# simplification, and an output on an image2d<int_u24>.
