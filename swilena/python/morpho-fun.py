#! /usr/bin/env python

# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
#
# This file is part of the Olena Library.  This library is free
# software; you can redistribute it and/or modify it under the terms
# of the GNU General Public License version 2 as published by the
# Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; see the file COPYING.  If not, write to
# the Free Software Foundation, 51 Franklin Street, Fifth Floor,
# Boston, MA 02111-1307, USA.

# \file python/morpho-fun.py
# \brief Test on mathematical morphology on functions (scalar images).

import data
from swilena import *

# Module alias.
image = image2d_int_u8

ima = image.load(data.lena)

dilated = image.dilation(ima, win_c4p())
image.save(dilated, "dilation.pgm")

eroded = image.erosion(ima, win_c4p())
image.save(eroded, "erosion.pgm")

image.save(image.opening(ima, c4()), "opening.pgm")
image.save(image.closing(ima, c4()), "closing.pgm")

image.save(image.gradient(ima, win_c4p()), "gradient.pgm")
image.save(image.gradient_internal(ima, win_c4p()), "gradient_internal.pgm")
image.save(image.gradient_external(ima, win_c4p()), "gradient_external.pgm")

image.save(image.closing_area(ima, c4(), 50), "closing_area.pgm")
image.save(image.closing_area(ima, c4(), 50), "opening_area.pgm")
