SOURCE=test.cc subsampling.cc
FLAG=-Wall -W -pedantic -I../../.. -g

all: sub gsub gau icp
	

sub:
	g++ subsampling.cc $(FLAG) -o '+sub.exe'

gsub:
	g++ gaussian_subsampling.cc $(FLAG) -o '+gsub.exe'

gau:
	g++ gaussian.cc $(FLAG) -o '+gau.exe'

run:
	time ./+sub.exe . . ; time ./+gsub.exe . .


icp:
	g++ icp.cc -I../../.. -g -o '+icp.exe'
icp++:
	g++ icp.cc -I../../.. -O3 -DNDEBUG -o '+icp.exe'

icp_max++:
	g++ icp_max.cc -I../../.. -O3 -DNDEBUG -pg  -o '+icp_map.exe'

icp_lazy++:
	g++ icp_lazy.cc -I../../.. -O3 -DNDEBUG -pg  -o '+icp_lazy.exe'


icp_sub:
	g++ icp_subsampled.cc -I../../.. -O3 -DNDEBUG -g -o '+icp_sub.exe'

icpsafe:
	g++ icp.cc -fsignaling-nans -ffloat-store -I../../.. -O1 -o '+icp.exe'

plot: icp
	./+icp.exe 01.pbm 02.pbm > log
	gnuplot plotscript

check: clean
	g++ icp.cc -I../../.. -O0 -o './bin/+icp_0'
	g++ icp.cc -I../../.. -O0 -ffloat-store -o './bin/+icp_0f'
	g++ icp.cc -I../../.. -O0 -DNDEBUG -o './bin/+icp_0D'
	g++ icp.cc -I../../.. -O0 -DNDEBUG -ffloat-store -o './bin/+icp_0Df'
	g++ icp.cc -I../../.. -O3 -o './bin/+icp_3'
	g++ icp.cc -I../../.. -O3 -ffloat-store -o './bin/+icp_3f'
	g++ icp.cc -I../../.. -O3 -DNDEBUG -o './bin/+icp_3D'
	g++ icp.cc -I../../.. -O3 -DNDEBUG -ffloat-store -o './bin/+icp_3Df'
	./icp_check.sh

clean:
	rm -f -- ./bin/*

.PHONY: check