#!/bin/zsh


process_file ()
{
  echo "Processing $1..." | cowsay
  input=$1
  dim=$2

  ./crop $input 0 50 90 149 230 170 crop.dump
  ./norm crop.dump norm.dump
  ./time_max_norm norm.dump

  convert tmax_max.pgm tmax_max.png
  convert tmax_smooth_max.pgm tmax_smooth_max.png
  convert tmax_time.pgm tmax_time.png
  convert tmax_smooth_time.pgm tmax_smooth_time.png
  rm tmax_*.pgm
}

# 3D (2D + t) images only
make crop norm time_max_norm
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" 3 52
