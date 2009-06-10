#! /usr/bin/env python

# Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

from swilena import *

# FIXME: See if could use cmd.Cmd here
# (http://docs.huihoo.com/pydoc/python/2.5/stdlib/cmd.Cmd-class.html).
class Example:
  def __init__(self, message):
    self.message = message
  def __repr__(self):
    return self.message

example = Example('''\
The following example creates a 2-d image of integers with 3 rows and
3 columns, then fills its cells with the value `42' and prints it:

  ima = image2d_int.image2d_int(3, 3)
  image2d_int.fill(ima, 42)
  image2d_int.println(ima)

You can try it by copying and pasting each line on the following prompt.
''')

print '''The Swilena Python Shell (sps).
Type "help", "copyright", "credits" or "license" for more information on Python
Type "example" for a short example.
Type "quit()" or ^D (Ctrl-D) to quit.'''
