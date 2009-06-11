# /bin/sh

if ! [ $# -eq 3 ]; then
  echo "Usage: $0 <file1> <file extension> <dst dir>"
  exit 1;
fi

begins=`grep -n -R '//[[:space:]]*\\\{' $1 | cut -d ':' -f 1`
ends=`grep -n -R '//[[:space:]]*\\\}' $1 | cut -d ':' -f 1`

split=false
i=1
for begin in $begins; do
  partend=$((`echo $ends | cut -d ' ' -f $i` - 1))
  subdir=`basename \`dirname $1\``
  out_name="`basename $1 .$2`-$i.$2"
  if [ "$subdir" != "examples" ] && [ "$subdir" != "outputs" ]; then
    out_name="${subdir}_$out_name"
  fi
  head -n $partend $1 | tail -n $(($partend - $begin)) > "$3/$out_name"
  i=$((i+1))
  split=true
done

if [ $split ] && [ $i -gt 1 ]; then
  echo "* $1 has been split in $i files."
fi
