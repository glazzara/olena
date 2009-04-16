#! /bin/sh

./+bin/distance_thick $1 out.pgm
convert -depth 8 +compress $1 ${1%pbm}pgm
./+bin/closing_rectangle out.pgm 11 101 out2.pgm 
./+bin/watershed_flooding out2.pgm out3.pgm 
./+bin/watershed_superpose ${1%pbm}pgm out3.pgm ${1%pbm}ppm
rm out.pgm out2.pgm out3.pgm
