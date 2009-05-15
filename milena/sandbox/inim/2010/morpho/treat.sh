#!/bin/sh



gaussian_force=20      # best with values around 20
k=5                    # best with values in [|1..10|]
window_width=20        # best with small values in [|10..30|]
lambda=5               # best with small values like 5-10
OUTPUT_DIR=output


if [ $# -eq 0 ]
then

    echo -n 'Cleaning output directory........'
    rm -f /lrde/beyrouth/lrde-2011/da-mota/images_INIM/result/*
    echo '......[DONE]'

    mkdir -p ${OUTPUT_DIR}

    for filename in `echo *.pbm`
    do
	output_filename=${OUTPUT_DIR}/${filename}

	~/morpho/morpho           \
	    ${filename}           \
	    ${gaussian_force}     \
	    ${k}                  \
	    ${window_width}       \
	    ${lambda}             \
	    ${output_filename}    \
	|| exit $?

    done

fi


for i in `ls ${OUTPUT_DIR}/*.pbm`
do
    display $i
done
