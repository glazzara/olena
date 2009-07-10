#!/bin/sh


process ()
{
  nlabels=`./filter_morpho_us $@`
#if [ $? -gt 0 ]; then
#   echo "error: filter_morpho_us failed"
#   exit
# fi

  ../bin/dump2pgm8b closing.dump closing.pgm
  convert closing.pgm 01_closing.png
  rm closing.dump closing.pgm

  ../bin/dump2pbm threshold.dump 3 threshold.pbm
  convert threshold.pbm 02_threshold.png
  rm threshold.dump threshold.pbm

  ../bin/dump2pbm opening.dump 3 opening.pbm
  convert opening.pbm 03_opening.png
  rm opening.dump opening.pbm

  ../bin/dump2pbm maj.dump 3 maj.pbm
  convert maj.pbm 05_maj.png
  rm maj.dump maj.pbm

#../bin/dumpl16_to_colorize blobs.dump 3 $nlabels blobs.ppm
# convert blobs.ppm 04_blobs.png
# rm blobs.dump blobs.ppm

  ../bin/dump2ppm output.dump output.ppm
  convert output.ppm 06_output.png
  rm output.dump output.ppm

  # Cleanup.
#rm *.pgm *.ppm
}


# $1 = input.dump
# $2 = lambda
# $3 = threshold
process $@
