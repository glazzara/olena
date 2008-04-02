#!/bin/sh

for i in `\ls bin/`
do
    echo execute $i 01.pbm 02.pbm
    ./bin/$i 01.pbm 02.pbm > ./bin/log_$i
    echo ./bin/log_$i
done
