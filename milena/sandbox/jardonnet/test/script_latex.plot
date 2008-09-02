set terminal latex
set output "eg1.tex"

set xlabel "q"
set ylabel "e"
set zlabel "time"
splot "log.dat"

