#!/bin/sh

out_location="./images-out"

treat_file()
{
    file=$1
    shift
    echo "-------------------------"
    echo " * Processing $file..."
    echo "  => Running: $1 $file"
    echo "-------------------------"
    d1=`date +%s`
    time $1 $file
    d2=`date +%s`
    echo ""
    dstdir="$out_location/`basename $file .pbm`"
    echo "Moving output files to $dstdir..."
    mkdir -p $dstdir
    mv -f *.pbm $dstdir 2>/dev/null
    mv -f *.ppm $dstdir 2>/dev/null
    mv -f *.pgm $dstdir 2>/dev/null
    echo ""
}

treat_dir()
{
  for file in $2/*; do
    treat_file $file $1
  done 
}

rm -f *.pbm *.ppm *.pgm
make

# Traite une seule image
# Usage: ./starh.sh <file.pbm> <table | photo>
if [ $# -eq 2 ]; then
   treat_file $1 ./$2
else
# Traite toutes les images
   treat_dir ./table ./images/factures
   treat_dir ./photo ./images/photos
fi
