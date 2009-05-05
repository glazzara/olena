# group.gp
# print epsilon evolution for a game at a fixed complexity


set term png enhanced size `echo "$w,$h"`

set style data boxes

set xlabel  "color"

set xrange  [0:255]
set yrange  [0:*]

set xtics  border nomirror
set ytics  border nomirror

set output "`echo "$out"`"

plot "`echo "$in"`" using 1:2 notitle
