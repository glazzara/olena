#! /bin/bash

conf="gplot.conf"

echo 'set terminal png;' > $conf
echo 'set size 2,1' >> $conf
echo 'set xtic out rotate by 90' >> $conf
i=0
for file in *.log; do
    printf "set output \"plot_%03i.png\"\n" $i >> $conf
    echo plot \"$file\" 'using 2:xticlabels(1) with lines' >> $conf
    i=$((i+1))
done

gnuplot $conf
