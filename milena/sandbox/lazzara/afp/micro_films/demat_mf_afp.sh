#!/bin/sh

PATH_TO_BIN=/lrde/stockholm/lazzara/svn/olena/git/oln/milena/sandbox/bin
PATH_TO_SCRIBO=/lrde/stockholm/lazzara/svn/olena/git/build/scribo/src

if [ $# -ne 2 ]; then
  echo "Usage: $0 <input.pgm> <initial rotation>"
  exit 1
fi

input=$PWD/$1
bname=`basename $1 .pgm`
mkdir -p $bname
cd $bname


echo "* Running pre-processing algorithms on $1."
${PATH_TO_SCRIBO}/binarization/simple $input ${bname}.pbm
${PATH_TO_BIN}/morpho/closing/structural_rectangle2d ${bname}.pbm 21 21 ${bname}_clo.pbm
./../keep_background ${bname}_clo.pbm ${bname}_bg.pbm
${PATH_TO_BIN}/morpho/dilation_rectangle2d ${bname}_bg.pbm 70 70 ${bname}_clo_dil.pbm
${PATH_TO_SCRIBO}/negate ${bname}_clo_dil.pbm ${bname}_clo_dil_i.pbm

echo "* Splitting $1 into several documents."
./../extract_mf $input ${bname}.pbm ${bname}_clo_dil_i.pbm "$2"

echo "* Processing extracted documents"
for f in mf_*_.pgm; do
  echo "  - Processing $f..."

  fbname=`basename $f .pgm`

# Compute two different binary versions.
  echo "    # Binarization."
  ${PATH_TO_BIN}/level_objects $f 100 ${fbname}_level.pgm
  ${PATH_TO_SCRIBO}/binarization/simple ${fbname}_level.pgm ${fbname}_level.pbm

  ${PATH_TO_SCRIBO}/binarization/simple $f ${fbname}.pbm

# '_level' or '' according to which binary version you want to use.
use_level=''

  ${PATH_TO_BIN}/logical/not ${fbname}${use_level}.pbm ${fbname}${use_level}_i.pbm

  echo "    # Filtering."
  ${PATH_TO_SCRIBO}/filter/small_objects ${fbname}${use_level}_i.pbm 3 ${fbname}${use_level}_small.pbm
  ${PATH_TO_SCRIBO}/filter/thin_objects ${fbname}${use_level}_small.pbm 3 ${fbname}${use_level}_small_thin.pbm

  echo "    # Text extraction."
  ${PATH_TO_SCRIBO}/extract_text_double_link ${fbname}${use_level}_small_thin.pbm 100 100 "${fbname}_text"
done

