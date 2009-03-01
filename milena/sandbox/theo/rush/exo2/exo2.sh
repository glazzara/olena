#! /bin/bash

if [ ! -f affiche2.pgm ]; then
    echo "affiche2.pgm introuvable"
    exit 1
fi

convert -geometry 10% affiche2.pgm temp.bmp
convert temp.bmp affiche2_10p.pgm
./exo2 affiche2_10p.pgm card -1 21
mv out.pbm card_10p.pbm
./exo2 affiche2.pgm card 2100 3000
mv out.pbm card.pbm
./exo2 affiche2.pgm sharpness 0.8 1
mv out.pbm sharpness.pbm
rm temp.bmp