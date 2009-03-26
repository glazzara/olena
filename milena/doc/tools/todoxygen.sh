#!/bin/sh

if [ $# -ne 2 ]; then
  echo "Usage: $0 <file.tex> <output dir>";
  exit 1;
fi

out="$2/`basename $1 .tex`.hh"
html="$2/`basename $1 .tex`.html"

#Convert .tex to .html
hevea -O -fix $1 -o $html

bodyl=`grep -n "<BODY >" $html | cut -d ':' -f 1`
nlines=`wc -l $html | cut -d ' ' -f 1`
tail_line=$(($nlines - $bodyl))

echo '/*! ' > $out
echo '\htmlonly' >> $out
tail -n $tail_line $html >> $out
echo '\endhtmlonly */' >> $out

sed -i -e 's/&#XA0/\&nbsp/g' $out
sed -i -e 's/&#X21A6/\&map/g' $out
sed -i -e 's/&#X2019/\&rsquo/g' $out
sed -i -e 's/&#X2192/\&rarr/g' $out
sed -i -e 's/&#X201C/\&ldquo/g' $out
sed -i -e 's/&#X201D/\&rdquo/g' $out
sed -i -e 's/&#X2261/\&equiv/g' $out

#Doxygen wants us to preserve '\n' after commands. 
#\ref and \see do not need that extra new line.
for keyword in include section page subpage subsection subsubsection image; do
  sed -i -e "s/\\\\endhtmlonly\\\\$keyword/\\\\endhtmlonly\n\n\\\\$keyword/g" $out
  sed -i -e "s/\\\\endhtmlonly\*\//\\\\endhtmlonly\n\n\*\/\n\n/g" $out
  sed -i -e "s/\\\\$keyword \(.*\)\\\\htmlonly/\\\\$keyword \1\n\n\\\\htmlonly\n/g" $out
done

