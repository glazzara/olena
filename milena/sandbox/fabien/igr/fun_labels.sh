#!/bin/zsh

process_file ()
{
  echo "Processing $3..."
  input=$1
  dim=$2

  ./grad $input $dim
  for lambda_closing in 5000; do
    echo "  for lambda_closing = ${lambda_closing}";
    ./clo_vol grad.dump $dim ${lambda_closing}
    nbasins=`./wst clo_vol.dump $dim`
    echo "    nbasins = $nbasins"
#../bin/dumpl32_to_colorize wst.dump $dim $nbasins colorize_${3}_${lambda_closing}.ppm
    ./fun_labels wst.dump $dim $input $nbasins
  done
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" 3 "52"
