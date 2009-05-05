#! /bin/sh
## graph.sh for memory
## Made by leblanc <nicuveo@lrde.epita.fr>
##


## Vars

in=$1
out=$2
w=$3
h=$4
if [ -z "$w" ] ; then
    w=800
fi
if [ -z "$h" ] ; then
    h=600
fi

## Code

export w
export h
export in
export out

gnuplot histo.gp
