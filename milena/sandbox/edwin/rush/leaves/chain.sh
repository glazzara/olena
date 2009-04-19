#! /bin/bash

function usage()
{
echo "./chain.sh input.ppm (--graylevel | --gradient)"
exit 2
}

if [ $# -ne 2 ]; then
    usage
fi

if [ "$2" = "--gradient" ]; then
    gradient=1
elif [ "$2" = "--graylevel" ]; then
    graylevel=1
else
    usage
fi

make leaves
if [ "x$gradient" = "x1" ]; then
    make all
    name="gradient.pgm"
    ./gradient "$1" "$name"
else
    name="$1"
    name="${name%.ppm}.pgm"
    convert +compress -depth 8 -colorspace Gray "$1" "$name"
fi
./leaves "$name"
rm -Rf "$1".dir
mkdir "$1".dir
mv *.txt "leaves.pbm" "$name" "$1".dir