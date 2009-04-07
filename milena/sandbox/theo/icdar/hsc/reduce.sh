#! /bin/sh

convert -negate -geometry 50% +compress -depth 8 $1 ${1%.pbm}_50p.pbm
convert -depth 8 +compress ${1%.pbm}_50p.pbm ${1%.pbm}_50p.pgm
