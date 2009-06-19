#!/bin/sh


process ()
{
  echo "Distance ${2}"

  ./dist_edges $1 -1 $2 ${4}_dist${2}.dump
  if [ $? -gt 0 ]; then
    echo "error: dist_edges failed"
    exit
  fi
  nbasins=`./wst_edges ${4}_dist${2}.dump $3`
  if [ $? -gt 0 ]; then
    echo "error: wst_edges failed"
    exit
  fi
  ./mean_median_label ${4}_dist${2}.dump watershed.dump $nbasins
  if [ $? -gt 0 ]; then
    echo "error: mean_median_label failed"
    echo "nbasins: $nbasins"
    exit
  fi

#convert dist.pgm dist${2}.png
#convert watershed.ppm watershed${2}.png
#convert means.pgm means${2}.png
#convert dist_mean.pgm dist_mean${2}.png
#convert dev.pgm dev${2}.png
  
  mv dist.pgm ${4}_dist${2}.pgm
  mv watershed.ppm ${4}_watershed${2}.pgm
  mv means.pgm ${4}_means${2}.pgm
  mv dist_mean.pgm ${4}_dist_mean${2}.pgm
  mv dev.pgm ${4}_dev${2}.pgm

  cp watershed.dump ${4}_watershed${2}.dump

  # Cleanup.
#rm *.pgm *.ppm
}

batch_process ()
{
  process $1 1 $2 $3
  process $1 2 $2 $3
  process $1 3 $2 $3
  process $1 4 $2 $3
  process $1 5 $2 $3
}

# $1 = input.dump
# $2 = closing lambda
# $3 = name
batch_process $1 500 $3
