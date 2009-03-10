all: table photo

tabledbg: demat.hh
	g++ -Wextra -Wall -I../.. -I../ -I$(HOME)/local/include -O1 -g src/table.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/table.out

table: demat.hh
	g++ -Wextra -Wall -I../.. -I. -I.. -I$(HOME)/local/include -O1 -DNDEBUG src/table.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/table.out

oldtable: demat.old.hh
	g++ -Wextra -Wall -I../.. -I../ -I$(HOME)/local/include -O1 -DNDEBUG src/table_old.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/table_old.out

photo: demat.hh
	g++ -Wextra -Wall -I../.. -I. -I.. -I$(HOME)/local/include -O1 -DNDEBUG src/photo.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/photo.out



extract_text_single_link:
	g++ -Wextra -Wall -I../.. -I../ -I$(HOME)/local/include -O1 -DNDEBUG src/extract_text_single_link.cc -o bin/extract_text_single_link.out

extract_text_double_link:
	g++ -Wextra -Wall -I../.. -I../ -I$(HOME)/local/include -O1 -DNDEBUG src/extract_text_double_link.cc -o bin/extract_text_double_link.out

extract_text_multiple_links:
	g++ -Wextra -Wall -I../.. -I../ -I$(HOME)/local/include -O1 -DNDEBUG src/extract_text_multiple_links.cc -o bin/extract_text_multiple_links.out


clean:
	rm *.ppm *.pgm *.pbm

dist-clean: clean
	rm -f bin/table.out bin/photo.out
