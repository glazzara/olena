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
	g++ icp.cc -ffloat-store -I../../.. -O3 -DNDEBUG -o '+icp.exe'

icpsafe:
	g++ icp.cc -fsignaling-nans -ffloat-store -I../../.. -O1 -o '+icp.exe'

run:
	time ./+sub.exe . . ; time ./+gsub.exe . .

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
	g++ icp.cc -I../../.. -O3 -DNDEBUG -ffloat-store -o './bin/+icp_3df'
	g++ icp_lazy.cc -I../../.. -O0 -o './bin/+icp_lazy_0'
	g++ icp_lazy.cc -I../../.. -O0 -ffloat-store -o './bin/+icp_lazy_0f'
	g++ icp_lazy.cc -I../../.. -O0 -DNDEBUG -o './bin/+icp_lazy_0D'
	g++ icp_lazy.cc -I../../.. -O0 -DNDEBUG -ffloat-store -o './bin/+icp_lazy_0Df'
	g++ icp_lazy.cc -I../../.. -O3 -o './bin/+icp_lazy_3'
	g++ icp_lazy.cc -I../../.. -O3 -ffloat-store -o './bin/+icp_lazy_3f'
	g++ icp_lazy.cc -I../../.. -O3 -DNDEBUG -o './bin/+icp_lazy_3D'
	g++ icp_lazy.cc -I../../.. -O3 -DNDEBUG -ffloat-store -o './bin/+icp_lazy_3df'

clean:
	rm -- ./bin/*

.PHONY: check