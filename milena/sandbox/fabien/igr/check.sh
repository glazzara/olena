#!/bin/zsh

process_file ()
{
  echo "Processing $3..."
  dist_max=10

  ./grad $1 $2
  for lambda_closing in 10 50 100 500 1000 5000 10000 50000; do
    echo "  for lambda_closing = ${lambda_closing}";
    ./clo_vol grad.dump $2 ${lambda_closing}
    nbasins=`./wst clo_vol.dump $2`
    ../bin/dumpl32_to_colorize wst.dump $2 $nbasins results/colorize_${3}_${lambda_closing}.ppm

    if [ ${lambda_closing} -eq 100 ]; then
      for lambda_dist in 10 50 100; do
	nbasins_after=`./graph wst.dump $2 $1 $lambda_dist $nbasins`
	mv wsd_original.pgm results/graph_${3}_${lambda_closing}_${lambda_dist}_01.pgm
	mv wsd_mean_colors.pgm results/graph_${3}_${lambda_closing}_${lambda_dist}_02.pgm
	echo "nbasins was" $nbasins "and now is" $nbasins_after "( diff =" $(($nbasins - $nbasins_after)) ")"
      done
    fi
  done
#  rm *.dump
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0049.dcm" 2 "49"
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" 3 "52"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0055.dcm" 2 "55"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm" 2 "58"
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm" 3 "61"
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm" 3 "64"
