# /bin/sh

# Copyright (C) 2008, 2009, 2010, 2013 EPITA Research and Development
# Laboratory (LRDE).
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

if ! [ $# -eq 4 ]; then
  echo "Usage: $0 <file1> <file extension> <suffix> <dst dir>"
  exit 1;
fi

begins=$(grep -n -h '//[[:space:]]*\\{' $1 | cut -d ':' -f 1)
ends=$(grep -n -h '//[[:space:]]*\\}' $1 | cut -d ':' -f 1)

if ! [ -z "$3" ]; then
  suffix=".$3"
fi

split=false
i=0
for begin in $begins; do
  i=$((i+1))
  partend=$((`echo $ends | cut -d ' ' -f $i` - 1))
  subdir=`basename \`dirname $1\``
  out_name="`basename $1 .$2`-$i.$2$suffix"
  if [ "$subdir" != "examples" ] && [ "$subdir" != "outputs" ]; then
    out_name="${subdir}_$out_name"
  fi
  head -n $partend $1 | tail -n $(($partend - $begin)) > "$4/$out_name"
  split=true
done

if [ $split ] && [ $i -gt 1 ]; then
  echo "* $1 has been split in $i files."
fi
