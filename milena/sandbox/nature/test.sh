#! /bin/sh

for i in $@;
do
    echo $i;
    echo "";
    ./nature $i;
    echo "";
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    echo "";
done;
