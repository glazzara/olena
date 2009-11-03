#! /usr/bin/env python

# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

# \file
# \brief A Python version of test/test_milena.cc using libdynmilena.

import ltihooks

import dyn
import mln

# We'd like to be able to write this:
#
#  ima = mln.mk_image2d_int(3, 3)
#
# but we just can't.  `mk_image2d_int' only accept `dyn.data' as
# arguments, so we have to encapsulate integers in `dyn.data' objects
# (likewise for strings).
#
# Moreover, mk_image2d_int is a global object (functor), registered as
# an attribute of the `mln.cvar' object.

ima = mln.cvar.mk_image2d_int(dyn.data(3), dyn.data(3))

mln.cvar.fill(ima, dyn.data(0))
mln.cvar.println(dyn.data("ima (before) ="), ima)
mln.cvar.iota(ima)
mln.cvar.println(dyn.data("ima (after) ="), ima)
