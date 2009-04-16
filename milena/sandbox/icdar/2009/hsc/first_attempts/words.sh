#! /bin/sh

./+bin/closing_rectangle   $1 5 11 tmp_small_clo.pbm
./+bin/distance_thick      tmp_small_clo.pbm tmp_dist.pgm
./+bin/closing_area        tmp_dist.pgm 500 tmp_clo.pgm     
./+bin/watershed_flooding  tmp_clo.pgm tmp_ws.pgm 
./+bin/watershed_superpose ${1%pbm}pgm tmp_ws.pgm ${1%pbm}ppm
