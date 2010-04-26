#! /usr/bin/env python

# Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

# \file python/morpho-fun.py
# \brief Test on mathematical morphology on functions (scalar images).

import data
from swilena import *

# Module alias.
image = image2d_int_u8

ima = image.load(data.lena)

dilated = image.dilation(ima, win_c4p())
image.save(dilated, "morpho-fun-dilation.pgm")

eroded = image.erosion(ima, win_c4p())
image.save(eroded, "morpho-fun-erosion.pgm")

image.save(image.opening(ima, c4()), "morpho-fun-opening.pgm")
image.save(image.closing(ima, c4()), "morpho-fun-closing.pgm")

image.save(image.gradient(ima, win_c4p()), "morpho-fun-gradient.pgm")
image.save(image.gradient_internal(ima, win_c4p()),
           "morpho-fun-gradient_internal.pgm")
image.save(image.gradient_external(ima, win_c4p()),
           "morpho-fun-gradient_external.pgm")

image.save(image.closing_area(ima, c4(), 50), "morpho-fun-closing_area.pgm")
image.save(image.closing_area(ima, c4(), 50), "morpho-fun-opening_area.pgm")
