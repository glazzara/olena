#! /bin/zsh

./+bin/elementary_gradient $1 1 tmp_grad.pgm
./+bin/closing_volume tmp_grad.pgm 500 tmp_clo.pgm
./+bin/watershed_flooding tmp_clo.pgm tmp_wst.pgm
./+bin/watershed_superpose $1 tmp_wst.pgm ${1%pgm}ppm

rm tmp_grad.pgm tmp_clo.pgm tmp_wst.pgm
