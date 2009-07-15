#!/bin/sh


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
