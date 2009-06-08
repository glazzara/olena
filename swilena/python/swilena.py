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

# \file swilena.py
# \brief The whole Swilena suite.

import ltihooks

from box2d import *
from point2d import *
from dpoint2d import *
from neighb2d import *
from window2d import *

from int_u8 import *
from int_u32 import *

import image2d_int
import image2d_int_u8
