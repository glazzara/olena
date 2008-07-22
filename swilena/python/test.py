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

import ltihooks
import image2d

ima = image2d.image2d_int(3, 3)
image2d.fill_image2d_int(ima, 42)

# FIXME: For the moment, we can't write
#
#   ima(image2d.point2d(i, j))
#
# but I don't know why.  Anyway, eventually we'd like to be able to
# write this:
#
#   for p in ima.domain():
#     print "ima(" + str(i) + ", " + str(j) + ") = " + ima(p)
#
# as it is generic and way closer to what we do in C++.
for i in range(0, 3):
  for j in range(0, 3):
    p = image2d.point2d(i, j)
    # FIXME: Handling POD types (like int) as value types is not
    # transparent: ima(p) returns a pointer to int wrapped in a SWIG
    # object, and cannot be easily converted to a Python integer
    # value.  Hence this explicit conversion using `intp_value'.
    v = image2d.intp_value(ima(p))
    print "ima(" + str(i) + ", " + str(j) + ") = " + str(v)

print
image2d.println_image2d_int(ima)
