#!/bin/zsh

for i in `\ls bin/`
do
    echo "  " $i
    \time -f "%U" ./bin/$i 01.pbm 02.pbm > ./bin/log_$i
    echo ""
done
