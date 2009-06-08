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

from swilena import *

ima = image2d_int.image2d_int(3, 3)
image2d_int.fill(ima, 42)

# FIXME: Help version of SWIG older than 1.3.39.  For instance, SWIG
# 1.3.36 does not generate code smart enough to detect that the output
# of `ima.domain()' should be a wrapped `box2d'.  Help it.
#
# We should get rid of this when SWIG 1.3.39 (or greater) is
# widespread.
if config.have_swig(0x010339):
  domain = ima.domain()
else:
  domain = box2d(3, 3)

# FIXME: Eventually we'd like to be able to write this:
#
#   for p in ima.domain():
#     print "ima(" + str(p) + ") = " + ima(p)
#
# as it is generic and way closer to what we do in C++.
for p in domain:
  # FIXME: Handling POD types (like int) as value types is not
  # transparent: ima(p) returns a pointer to int wrapped in a SWIG
  # object, and cannot be easily converted to a Python integer
  # value.  Hence this explicit conversion using `intp_value'.
  v = image2d_int.intp_value(ima(p))
  print "ima" + str(p) + " = " + str(v)

# FIXME: This is too complicated.  We should be able to write
# 
#   ima(1,1) = 51
#
# or at least
#
#  ima.set(1, 1, 51)
#
image2d_int.intp_assign(ima(point2d(1,1)), 10)

print
image2d_int.println(ima)

# Elementary erosion in 4-connectivity using win_c4p.
eroded = image2d_int.erosion (ima, win_c4p())
image2d_int.println(eroded)

# Erosion using a custom window.
w = window2d()
w.insert(-1, 0)
w.insert( 0, 0)
w.insert(+1, 0)
eroded = image2d_int.erosion (ima, w)
image2d_int.println(eroded)
