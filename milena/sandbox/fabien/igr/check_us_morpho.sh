#!/bin/sh


process ()
{
  ./grad $1 3 ${2}_us_grad.dump
  if [ $? -gt 0 ]; then
    echo "error: grad failed"
    exit
  fi

  for lambda in 500 1000 2000; do

    ./clo_vol ${2}_us_grad.dump 3 $lambda ${2}_us_clo_${lambda}.dump
    if [ $? -gt 0 ]; then
      echo "error: clo_vol failed"
      exit
    fi

    nbasins=`./wst ${2}_us_clo_${lambda}.dump 3 ${2}_us_wst_${lambda}.dump`
    if [ $? -gt 0 ]; then
      echo "error: wst failed"
      echo "nbasins: $nbasins"
      exit
      fi

#./maj ${2}_us_wst_${lambda}.dump ${2}_us_maj_${lambda}.dump
#   if [ $? -gt 0 ]; then
#     echo "error: maj failed"
#     exit
#   fi

    ../bin/dumpl16_to_colorize ${2}_us_wst_${lambda}.dump 3 $nbasins ${2}_us_maj_${lambda}.ppm
    convert ${2}_us_maj_${lambda}.ppm ${2}_us_maj_${lambda}.png
    rm ${2}_us_maj_${lambda}.ppm

  done

  # Cleanup.
#rm *.pgm *.ppm
}


# $1 = input.dump
# $2 = name
process $1 $2
