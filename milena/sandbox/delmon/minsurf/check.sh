#! /bin/sh

for i in *; do
  BASENAME=`echo $i | cut -d '_' -f 1 | cut -d '/' -f 2`
  NUMBER=`echo $i | cut -d '_' -f 2`
  echo ${BASENAME}_${NUMBER}
  ./minsurf img/${BASENAME}.pgm $i res/${BASENAME}_${NUMBER}_r.pgm
  ./keep_object img/${BASENAME}.pgm res/${BASENAME}_${NUMBER}_r.pgm ext/${BASENAME}_${NUMBER}_e.pgm
done
