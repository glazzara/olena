#!/bin/zsh

process_file ()
{
  echo "Processing $2..."
  box_size=2

  for lambda_closure in 10 50 100 500 1000; do
    echo "  for lambda_closure = ${lambda_closure}";
    
    for dist_max in 5 10 50 100 500; do
      echo "    for lambda_dist = ${dist_max}";

      ./wsd3d $1 $lambda_closure $box_size $dist_max

      ../bin/dump2pgm8b wsd_01_src.dump results/${2}_${lambda_closure}_${dist_max}_01.pgm
      ../bin/dump2pgm8b wsd_02.dump results/${2}_${lambda_closure}_${dist_max}_02.pgm
      ../bin/dump2pgm8b wsd_03.dump results/${2}_${lambda_closure}_${dist_max}_03.pgm
      ../bin/dump2pgm8b wsd_04.dump results/${2}_${lambda_closure}_${dist_max}_04.pgm
      ../bin/dump2pgm8b wsd_05.dump results/${2}_${lambda_closure}_${dist_max}_05.pgm
      ../bin/dump2pgm8b wsd_06_mean_colors.dump results/${2}_${lambda_closure}_${dist_max}_06.pgm
#../bin/dump2pgm8b wsd_07_graph_image2_white.dump results/${2}_${lambda_closure}_${dist_max}_07.pgm
#../bin/dump2pgm8b wsd_08_graph_image2_black.dump results/${2}_${lambda_closure}_${dist_max}_08.pgm
      ../bin/dump2pgm8b wsd_99_result.dump results/${2}_${lambda_closure}_${dist_max}_99.pgm

      rm *.dump

      done;
  done
}

make wsd3d
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" "52"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm" "61"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm" "64"
