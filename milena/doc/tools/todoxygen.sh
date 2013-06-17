#! /bin/sh

# todoxygen.sh: Turn HTML files into suitable inputs for doxygen.

# Copyright (C) 2009, 2010, 2013 EPITA Research and Development
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

test $# -eq 3 \
  || { echo "Usage: $0 <input.html> <output.hh> <path to .sty>"; exit 1; }

html="$1"
out="$2"

bodyl=`grep -n -i "<BODY" $html | cut -d ':' -f 1`
nlines=`wc -l $html | cut -d ' ' -f 1`
tail_line=$(($nlines - $bodyl))

echo '/*! ' > $out
echo '\htmlonly' >> $out
tail -n $tail_line $html >> $out
echo '\endhtmlonly */' >> $out

perl -pi -e 's/&#XA0/\&nbsp/g' $out
perl -pi -e 's/&#X21A6/\&map/g' $out
perl -pi -e 's/&#X2019/\&rsquo/g' $out
perl -pi -e 's/&#X2192/\&rarr/g' $out
perl -pi -e 's/&#X201C/\&ldquo/g' $out
perl -pi -e 's/&#X201D/\&rdquo/g' $out
perl -pi -e 's/&#X2261/\&equiv/g' $out

# Doxygen wants us to preserve '\n' after commands. 
# \ref and \see do not need that extra new line.
for keyword in include section page subpage subsection subsubsection image; do
  perl -pi -e "s/\\\\endhtmlonly\\\\$keyword/\\\\endhtmlonly\n\n\\\\$keyword/g" $out
  perl -pi -e "s/\\\\endhtmlonly\*\//\\\\endhtmlonly\n\n\*\/\n\n/g" $out
  perl -pi -e "s/\\\\$keyword \(.*\)\\\\htmlonly/\\\\$keyword \1\n\n\\\\htmlonly\n/g" $out
done
