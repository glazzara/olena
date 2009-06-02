#!/bin/sh


process ()
{
  ./dist_edges $1 -1 $2 dist${2}.dump
  if [ $? -gt 0 ]; then
    echo "error: dist_edges failed"
    exit
  fi
  nbasins=`./wst_edges dist${2}.dump $3`
  if [ $? -gt 0 ]; then
    echo "error: wst_edges failed"
    exit
  fi
  ./mean_median_label dist${2}.dump watershed.dump $nbasins
  if [ $? -gt 0 ]; then
    echo "error: mean_median_label failed"
    echo "nbasins: $nbasins"
    exit
  fi

  convert dist.pgm dist${2}.png
  convert watershed.ppm watershed${2}.png
  convert means.pgm means${2}.png
  convert dist_mean.pgm dist_mean${2}.png
  convert dev.pgm dev${2}.png
  
  cp watershed.dump watershed${2}.dump

  # Cleanup.
  rm *.pgm *.ppm
}

batch_process ()
{
  process $1 1 $2
  process $1 2 $2
  process $1 3 $2
  process $1 4 $2
}

batch_process $1 $2
