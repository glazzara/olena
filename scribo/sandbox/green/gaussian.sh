#!/bin/sh

############
# gaussian #
############

gnuplot <<EOF
set terminal x11 persist 1
square(x)=x*x
gaussian(x,mean,sigma)=exp(-0.5*square((x-mean)/sigma))/(sigma*sqrt(2*pi))
plot [-20:20] gaussian(x,0,6)
EOF
