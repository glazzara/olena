#! /bin/sh

bin_rep='/home/theo/lrde/olena/trunk/milena/sandbox/theo/+bin'

convert -geometry 25% +compress -depth 8 $1 tmp_small.pgm
convert -negate -geometry 25% +compress $1 tmp_small.pbm
$bin_rep/closing_rectangle   tmp_small.pbm 1 7 tmp_small_clo.pbm
$bin_rep/distance_thick      tmp_small_clo.pbm tmp_dist.pgm
$bin_rep/closing_area        tmp_dist.pgm 125 tmp_clo.pgm     
$bin_rep/watershed_flooding  tmp_clo.pgm tmp_ws.pgm 
$bin_rep/watershed_superpose tmp_small.pgm tmp_ws.pgm tmp_out.ppm
