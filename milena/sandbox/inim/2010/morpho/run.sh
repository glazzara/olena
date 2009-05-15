#!/bin/sh

gaussian_force=20      # best with values around 20
k=5                    # best with values in [|1..10|]
window_width=20        # best with small values in [|10..30|]
lambda=5               # best with small values like 5-10

if [ $# -ne 2 ]
then
    echo "Usage: ./run.sh <input_image.pbm> <output_image.pgm>"
    exit 1
fi

./morpho                  \
    $1                    \
    ${gaussian_force}     \
    ${k}                  \
    ${window_width}       \
    ${lambda}             \
    $2                    \
    || exit $?
