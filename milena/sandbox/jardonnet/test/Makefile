SOURCE=test.cc subsampling.cc
FLAG=-Wall -W -pedantic -I../../.. -g

all: 
	echo "sub gsub gau icpD icp check bench"

sub:
	g++ subsampling.cc $(FLAG) -o 'sub'

gsub:
	g++ gaussian_subsampling.cc $(FLAG) -o 'gsub'

gau:
	g++ gaussian.cc $(FLAG) -o 'gau'

icpD: icp.cc
	g++ icp.cc -I../../.. -g -o 'icpD'
icp: icp.cc
	g++ icp.cc -I../../.. -O3 -DNDEBUG -o 'icp'

bench:
	./check.rb

clean:
	rm -f -- ./bin/*
	rm -f sub gsub gau icpD icp
	rm -f log.dat registred.pbm

.PHONY: check