#! /usr/bin/env python

# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

from swilena import *

# Generic iterator interface.

b = box2d(2, 3)
p = iter(b)
while p.is_valid():
  print p.site()
  p.advance()

print

# Python's iterator interface.

# We cannot use
#
#   for p in box2d(2, 3):
#     print p
#
# here because the box2d is a temporary object that may be collected
# before the end of the iteration.  To prevent Python from disposing
# of it, we use a named variable that will
#
# Another possibility would be to have a generator playing with the
# `thisown' field of the box, to prevent its destruction (see
# http://www.swig.org/Doc1.3/SWIGDocumentation.html#Python_nn30).

for p in b:
  print p
