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

icpD: icp.cc +depend
	g++ icp.cc -I../../.. -g -o 'icpD'

icp: icp.cc +depend icp.o
	g++ icp.o -I../../.. -O3 -DNDEBUG -o 'icp'

icp_ref:
	g++ icp_ref.cc -I../../.. -g -o 'icp_ref'

icp.o:
	g++ -c icp.cc -I../../.. -O3 -DNDEBUG 

bench:
	ruby bench.rb

clean:
	rm -f -- ./bin/*
	rm -f sub gsub gau icpD icp
	rm -f log.dat registred.pbm
	rm -f i_*
	rm -f tmp.ppm registred.ppm semantic.cache

.PHONY: check bench icpD

+depend:
	g++ -M icp.cc $(FLAG) > +depend

include +depend