#!/bin/sh

for i in `ls *.tif`; do
    ../closure $i.pbm 1 $i.pbm ;
done
