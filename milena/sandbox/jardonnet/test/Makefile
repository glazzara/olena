SOURCE=test.cc subsampling.cc
FLAG=-Wall -W -pedantic -I../../.. -g

all: sub gsub gau icp
	

sub:
	g++ subsampling.cc $(FLAG) -o '+sub.exe'

gsub:
	g++ gaussian_subsampling.cc $(FLAG) -o '+gsub.exe'

gau:
	g++ gaussian.cc $(FLAG) -o '+gau.exe'

icp:
	g++ icp.cc $(FLAG) -o '+icp.exe'

icp++:
	g++ icp.cc -ffloat-store -I../../.. -O3 -DNDEBUG -o '+icp.exe'

icpsafe:
	g++ icp.cc -fsignaling-nans -ffloat-store -I../../.. -O1 -o '+icp.exe'

run:
	time ./+sub.exe . . ; time ./+gsub.exe . .

plot: icp
	./+icp.exe 01.pbm 02.pbm > log
	gnuplot plotscript

