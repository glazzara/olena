#! /usr/bin/env python

# Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

# \file python/mesh-complex-segm.py
# \brief Test complex2.
#
# See also Milena's tests/topo/complex.cc.

from swilena import *

# A 2-d (simplicial) complex and its adjacency graph.
#
#          v0      e3     v3
#            o-----------o                v0----e3----v3
#           / \ ,-----. /                / \    |    /
#          / . \ \ t1/ /                /   \   t1  /
#      e0 / / \ e1\ / / e4             e0.  ,e1'  `e4
#        / /t0 \ \ ' /                /   t0  \   /
#       / `-----' \ /                /    |    \ /
#      o-----------o                v1----e2----v2
#   v1      e2      v2
#
#   v = vertex   (0-face)
#   e = edge     (1-face)
#   t = triangle (2-face)

## ---------------------- ##
## Complex construction.  ##
## ---------------------- ##

c = complex2()

# 0-faces (points).
v0 = c.add_face()
v1 = c.add_face()
v2 = c.add_face()
v3 = c.add_face()

# 1-faces (segments).
e0 = c.add_face(-v1 + v0)
e1 = c.add_face(-v0 + v2)
e2 = c.add_face(-v2 + v1)
e3 = c.add_face(-v0 + v3)
e4 = c.add_face(-v3 + v2)

# 2-faces (triangles).
t0 = c.add_face( e0 + e1 + e2)
t1 = c.add_face(-e1 + e3 + e4)

print c


## ------------------ ##
## Handles and data.  ##
## ------------------ ##

# Get the face data from (``static'') face handle E0.
face1 = e0.data()

# Face handle.
f = face_2(e0)
print f
print

# Get the face data from (``dynamic'') face handle AF.
face2 = f.data_1()


## ----------- ##
## Iteration.  ##
## ----------- ##

# --------------- #
# Iterator on C.  #
# --------------- #

# (Forward) Iterator on a complex (not complex_image), or more
# precisely on (all) the faces of complex C.
for f in c:
  print f


# FIXME: Test more iterators.
