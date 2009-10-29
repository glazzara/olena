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
# \brief Configuration values of the package.

# FIXME: We might want to turn this into a config.py.in file and
# generate it instead of getting variables from the environment.

import os

abs_top_srcdir = os.environ["abs_top_srcdir"]
abs_milena_dir = os.path.join(abs_top_srcdir, "..", "milena")
