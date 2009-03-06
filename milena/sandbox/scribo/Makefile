all: table photo

tabledbg: demat.hh
	g++ -Wextra -Wall -I../.. -I$(HOME)/local/include  -O1 -g table.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o table

table: demat.hh
	g++ -Wextra -Wall -I../.. -I$(HOME)/local/include  -O1 -DNDEBUG table.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o table

photo: demat.hh
	g++ -Wextra -Wall -I../.. -I$(HOME)/local/include  -O1 -DNDEBUG photo.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o photo

clean:
	rm *.ppm *.pgm *.pbm

dist-clean: clean
	rm -f table photo
