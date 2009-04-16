#! /bin/sh

convert -geometry 25% -depth 8 +compress $1 small.pgm 
./+bin/gaussian_directional_2d__float small.pgm 1 1 31 fuz.pgm

./+bin/closing_rectangle fuz.pgm 7 31 tmp.pgm
./+bin/watershed_flooding tmp.pgm ws_txt.pgm

convert -negate fuz.pgm -depth 8 +compress tmp.pgm

./+bin/closing_rectangle tmp.pgm 7 31 tmp.pgm
./+bin/watershed_flooding tmp.pgm ws_spc.pgm

./+bin/relabel_lines ws_txt.pgm ws_spc.pgm tmp.ppm

./+bin/show_lines small.pgm tmp.ppm ${1%pbm}ppm

rm fuz.pgm tmp.pgm ws_txt.pgm ws_spc.pgm small.pgm tmp.ppm
