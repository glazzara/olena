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


#  # icp
icpD: icp.cc +depend
	g++ icp.cc -I../../.. -g -o 'icpD'
icp: +depend
	g++ icp.cc -I../../.. -O3 -DNDEBUG -o 'icp'


# # icp-ref
icp_refD: icp_ref.cc ../registration/icp_ref.hh +depend
	g++ -Wall -W icp_ref.cc -I../../.. -g -o 'icp_refD'
icp_ref: icp_ref.cc ../registration/icp_ref.hh +depend
	g++ icp_ref.cc -I../../.. -O3 -DNDEBUG -o 'icp_ref'


# # reg
regD: registration.cc +depend
	g++ registration.cc -I../../.. -g -o 'reg'
reg: registration.cc +depend
	g++ registration.cc -I../../.. -g -o 'reg'

bench:
	ruby bench.rb

test: icp icp_ref
	ruby test.rb

clean:
	rm -f -- ./bin/*
	rm -f sub gsub gau icpD icp icp_refD icp_ref *.o
	rm -f log.dat registred.pbm
	rm -f i_*
	rm -f tmp.ppm registred.ppm semantic.cache
	rm +depend
	rm -f step_*

.PHONY: check bench test icpD icp_refD icp icp_ref

+depend:
	g++ -MM icp.cc $(FLAG) > +depend

include +depend
