#!/bin/zsh


create_label_anim ()
{
  echo "      Creating label ${4} animation..."
  ./label2gif $2 $1 $3 $4
  for i in debug_label_${4}_*.pgm; do
    j=${i:r}.png
    convert $i $j
    rm $i
    sips -r 90 $j -o $j > /dev/null 2> /dev/null
    sips -f horizontal $j -o $j > /dev/null 2> /dev/null
  done
  convert -delay 10 -loop 0 debug_label_${4}_*.png debug_${5}_label_${4}.gif
  rm debug_label_${4}_*.png
}

create_all_labels ()
{
  count=0;
  while [ $count -lt $3 ]; do
    create_label_anim $1 $2 $3 $count $4
    count=$(($count + 1))
  done
}

usage ()
{
  echo "./all_labels2gif.sh ima ima_labels nlabels closing"
}

create_all_labels $1 $2 $3 $4
