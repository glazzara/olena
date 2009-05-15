#! /usr/bin/env python

# Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
#
# As a special exception, you may use this file as part of a free
# software library without restriction.  Specifically, if other files
# instantiate templates or use macros or inline functions from this
# file, or you compile this file and link it with other files to
# produce an executable, this file does not by itself cause the
# resulting executable to be covered by the GNU General Public
# License.
# reasons why the executable file might be covered by the GNU General
# Public License.

# \file python/morpho-segm.py
# \brief Test on watershed transform-based segmentation.

import data
from swilena import *

# Module alias.
image = image2d_int_u8

ima = image.load(data.lena)

# Gradient.
gradient = image.gradient(ima, win_c4p())
image.save(gradient, "gradient.pgm")
# Area closing of the gradient.
closed_gradient = image.closing_area(ima, c4(), 50)
# Watershed transform.
nbasins = int_u8();
ws = image.meyer_wst (closed_gradient, c4(), nbasins)
print nbasins
image.save(ws, "segm.pgm")

# FIXME: Also re-enable the naive segmentation with no gradient
# simplification, and an output on an image2d<int_u32>.
