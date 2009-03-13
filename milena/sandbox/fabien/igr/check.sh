#!/bin/zsh

process_file ()
{
  echo "Processing $3..."
  dist_max=10
  input=$1
  dim=$2

  ./grad $input $dim
  for lambda_closing in 50 100 500 1000 5000 10000 50000; do
    echo "  for lambda_closing = ${lambda_closing}";
    ./clo_vol grad.dump $dim ${lambda_closing}
    nbasins=`./wst clo_vol.dump $dim`
    ../bin/dumpl32_to_colorize wst.dump $dim $nbasins results/colorize_${3}_${lambda_closing}.ppm
    median=`./med wst.dump $dim $input $nbasins`
    ../bin/dumpi12_to_pgm med.dump $dim results/median_${3}_${lambda_closing}.pgm
    ./thres med.dump $dim $median
    mv bin_result.pbm results/result_${3}_${lambda_closing}.pbm

#if [ $2 -eq 2 ]; then
#     if [ ${lambda_closing} -eq 100 ]; then
#	for lambda_dist in 50 100 120 130 140 150; do
#	  nbasins_after=`./graph wst.dump $2 $1 $lambda_dist $nbasins`
#	  mv wsd_original.pgm results/graph_${3}_${lambda_closing}_${lambda_dist}_01.pgm
#	  mv wsd_mean_colors.pgm results/graph_${3}_${lambda_closing}_${lambda_dist}_02.pgm
#	  mv wsd_colorize_01.ppm results/graph_${3}_${lambda_closing}_${lambda_dist}_03.ppm
#	  mv wsd_colorize_02.ppm results/graph_${3}_${lambda_closing}_${lambda_dist}_04.ppm
#	  diff=$(($nbasins - $nbasins_after))
#	  echo "    dist = $lambda_dist | $nbasins -> $nbasins_after (diff = $diff ) $(($diff * 100 / $nbasins))%"
#	done
#     fi
#   fi
  done
#  rm *.dump
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0049.dcm" 2 "49"
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" 3 "52"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0055.dcm" 2 "55"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm" 2 "58"
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm" 3 "61"
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm" 3 "64"
