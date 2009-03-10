#!/bin/zsh

process_file ()
{
  echo "Processing $2..."
  box_size=2

  for lambda_closure in 10 50 100 500 1000; do
    echo "  for lambda_closure = ${lambda_closure}";
    
    for dist_max in 5 10 50 100 500; do
      echo "    for lambda_dist = ${dist_max}";

      ./wsd2d $1 $lambda_closure $box_size $dist_max

      mv wsd_01_src.pgm results/${2}_${lambda_closure}_${dist_max}_01.pgm
      mv wsd_02.pgm results/${2}_${lambda_closure}_${dist_max}_02.pgm
      mv wsd_03.pgm results/${2}_${lambda_closure}_${dist_max}_03.pgm
      mv wsd_04.pgm results/${2}_${lambda_closure}_${dist_max}_04.pgm
      mv wsd_05.pgm results/${2}_${lambda_closure}_${dist_max}_05.pgm
      mv wsd_06_mean_colors.pgm results/${2}_${lambda_closure}_${dist_max}_06.pgm
      mv wsd_07_graph_image2_white.pgm results/${2}_${lambda_closure}_${dist_max}_07.pgm
      mv wsd_08_graph_image2_black.pgm results/${2}_${lambda_closure}_${dist_max}_08.pgm
      mv wsd_99_result.pgm results/${2}_${lambda_closure}_${dist_max}_99.pgm

      done;
  done
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0049.dcm" "49"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0055.dcm" "55"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm" "58"
