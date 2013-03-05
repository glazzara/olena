#! /usr/bin/env python

# Copyright (C) 2008, 2009, 2013 EPITA Research and Development
# Laboratory (LRDE)
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

# \file swilena.py
# \brief The whole Swilena suite.

import ltihooks

import config

from box2d import *
from point2d import *
from dpoint2d import *
from neighb2d import *
from window2d import *

from int_u8 import *
from int_u24 import *

import image2d_int
import image2d_int_u8
