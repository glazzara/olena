#! /bin/bash
FILE=gplot.batch

echo "set terminal png" > "$FILE"
for f in *.txt; do
    echo 'set output "'$f'.png"' >> "$FILE"
    echo 'plot "'$f'"' >> "$FILE"
done;

gnuplot "$FILE"