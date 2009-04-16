#!/bin/zsh

reconstruct_plot ()
{
  rm -f ${1}_${2}.plot
  for i in ${1}_*_label*; do
    cat $i >> ${1}_${2}.plot
  done
}

rename_label_plots ()
{
  for i in label_*.plot; do
    mv $i ${1}_$i
  done
}

create_anim ()
{
  echo "      Creating ${1} animation..."
  for i in debug_${1}_*.pgm; do
    j=${i:r}.png
    convert $i $j
    rm $i
    sips -r 90 $j -o $j > /dev/null 2> /dev/null
    sips -f horizontal $j -o $j > /dev/null 2> /dev/null
  done
  convert -delay 10 -loop 0 debug_${1}_*.png debug_${1}_${2}.gif
  rm debug_${1}_*.png
}

create_label_anim ()
{
  echo "      Creating ${1} animation..."
  for i in debug_${1}_*.pgm; do
    j=${i:r}.png
    convert $i $j
    rm $i
    sips -r 90 $j -o $j > /dev/null 2> /dev/null
    sips -f horizontal $j -o $j > /dev/null 2> /dev/null
  done
  convert -delay 10 -loop 0 debug_${1}_*.png debug_${1}_${2}.gif
  rm debug_${1}_*.png
}

process_file ()
{
  echo "Processing $3..."
  input=$1
  dim=$2

  ./crop $input 0 50 90 149 230 170 crop.dump
  ./grad crop.dump $dim
  for lambda_closing in 5000; do
    echo "  for lambda_closing = ${lambda_closing}";
    ./clo_vol grad.dump $dim ${lambda_closing}
    nbasins=`./wst clo_vol.dump $dim`
    echo "    nbasins = $nbasins"

#./mean_slices wst.dump $input 
#../bin/dumpi12_to_png mean_slices.dump $dim mean_slices_${3}_${lambda_closing}.png
#../bin/dumpi12_to_pgm mean_slices.dump $dim mean_slices_${3}_${lambda_closing}.pgm

    ./fun_labels wst.dump crop.dump $nbasins
    rename_label_plots ${lambda_closing}

    ./all_labels2gif.sh ima.dump labels.dump $nbasins ${lambda_closing}

    mv *.gif results/plots/
    mv *.plot results/plots/

#reconstruct_plot tumeur ${lambda_closing}
#create_anim tumeur ${lambda_closing}
#reconstruct_plot air ${lambda_closing}
#create_anim air ${lambda_closing}
#reconstruct_plot poumon ${lambda_closing}
#create_anim poumon ${lambda_closing}

  done
}

# 3D (2D + t) images only
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" 3 52
