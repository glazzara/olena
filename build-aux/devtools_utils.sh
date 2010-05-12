#!/bin/sh

# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
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


# $1: a file.
#
# Return the absolute file path.
#
abs_filepath()
{
  path=$PWD
  if [ "`dirname $1`" != "." ]; then
    path=$path/`dirname $1`
  fi

  echo "$path"
}


# $1: a file.
#
# Return the relative path to mln
#
mln_based_path()
{
    path=`abs_filepath "$1"`
    local mln_path=""
    while [ "`basename $path`" != "mln" ]; do
	mln_path="`basename $path`/$mln_path"
	path=`dirname $path`
    done
    mln_path="mln/$mln_path"

    echo "$mln_path" | sed -e 's,/$,,'
}
