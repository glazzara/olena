#!/bin/sh

############
# gaussian #
############

# support 6 sigma sur chaque axe

gnuplot <<EOF
set terminal x11 persist 1
square(x)=x*x
gaussian(x,mean,sigma)=exp(-0.5*square((x-mean)/sigma))/(sigma*sqrt(2*pi))
gaussian2d(x,y,mx,my,sx,sy)=gaussian(x,mx,sx)*gaussian(y,my,sy)


set view 50
#set grid
#set contour
set isosample 20
splot [x=-20:20][y=-20:20] gaussian2d(x,y,0,0,6,6) with line palette
EOF
