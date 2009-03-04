#! /bin/sh

for i in sel/*; do
  BASENAME=`echo $i | cut -d '_' -f 1 | cut -d '/' -f 2`
  NUMBER=`echo $i | cut -d '_' -f 2`
  OCOLOR=`echo $i | cut -d '_' -f 3`
  echo ${BASENAME}_${NUMBER}
  ./growcut_bw img/${BASENAME}.pgm $i res/${BASENAME}_${NUMBER}_r.pgm
  if [ "x$OCOLOR" = "xw" ] ; then
    ./keep_white img/${BASENAME}.pgm res/${BASENAME}_${NUMBER}_r.pgm ext/${BASENAME}_${NUMBER}_e.pgm 
  elif [ "x$OCOLOR" = "xg" ] ; then
    ./keep_grey img/${BASENAME}.pgm res/${BASENAME}_${NUMBER}_r.pgm ext/${BASENAME}_${NUMBER}_e.pgm 
  elif [ "x$OCOLOR" = "xb" ] ; then
    ./keep_black img/${BASENAME}.pgm res/${BASENAME}_${NUMBER}_r.pgm ext/${BASENAME}_${NUMBER}_e.pgm 
  fi
done
