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

import os
from swilena import *

# Factor this part in a shared test Python file.
top_srcdir = os.environ["top_srcdir"]
img_dir = os.path.join(top_srcdir, "milena", "img")
lena = os.path.join (img_dir, "lena.pgm")

# Module alias.
image = image2d_int_u8

ima = image.load(lena)

eroded = image.erosion(ima, win_c4p())
image.save(eroded, "eroded.pgm")

dilated = image.dilation(ima, win_c4p())
image.save(dilated, "dilated.pgm")

# Gradient.
gradient = image.gradient(ima, win_c4p())
# Area closing.
closed_gradient = image.image2d_int_u8(gradient.domain())
image.closing_area(ima, c4(), 50, closed_gradient)
# Watershed transform.
nbasins = int_u8();
ws = image.meyer_wst (closed_gradient, c4(), nbasins)
# FIXME: Actualy print the number of basins; for the moment, this
# statement outputs something like
#
#   <int_u32.int_u32; proxy of <Swig Object of type 'mln::value::int_u< 32 > *'
#    at 0x816e160> >
#
print nbasins
image.save(ws, "ws.pgm")
