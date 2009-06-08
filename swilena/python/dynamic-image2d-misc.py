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

# \file python/dynamic-image2d-misc.py
# \brief Test on C++ ``dynamic'' image2d extended in Python.

from swilena import *
import dynamic_image2d

# Shortcuts.
image = dynamic_image2d
dyn_ima2d = image.dynamic_image2d_int_u8


# A simple image extending dynamic_image2d_int_u8.
class simple_image(dyn_ima2d):

  # FIXME: Implement other ctors.
  def __init__(self, b):
    dyn_ima2d.__init__(self, b)
    # FIXME: There must be a better way...
    self.values_ = []
    for n in range(0, b.nrows() * b.ncols()):
      self.values_.append(int_u8())
  
  def __call__(self, p):
    # FIXME: This assumes the box starts at (0, 0), which is not
    # always true!
    off = p.row() * self.domain_.ncols() + p.col()
    return self.values_[off]

  def is_valid(self):
    return True


b = box2d(10, 10)
ima = simple_image(b)

image.fill(ima, int_u8(42))
image.println(ima)

for p in ima.domain():
    print ima(p)
