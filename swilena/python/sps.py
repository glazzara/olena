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
