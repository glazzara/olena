#!/bin/zsh

process_file ()
{
  echo "Processing $2..."

  for lambda_closure in 500 1000 5000 10000 50000; do
    echo "  for lambda_closure = ${lambda_closure}";
    ./nbasins_finder $1 $lambda_closure
    ../bin/dump2ppm result_nbasins.dump results/nbasins_${2}_${lambda_closure}.ppm
  done
}

#make nbasins
#process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" "52"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm" "61"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm" "64"
