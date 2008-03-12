SOURCE=test.cc subsampling.cc
FLAG=-Wall -W -I../../.. -g

all: sub gsub
	

sub:
	g++ subsampling.cc $(FLAG) -o '+sub.exe'

gsub:
	g++ gaussian_subsampling.cc $(FLAG) -o '+gsub.exe'

gau:
	g++ gaussian.cc $(FLAG) -o '+gau.exe'

run:
	time ./+sub.exe . . ; time ./+gsub.exe . .