CXX_FLAGS = -Wextra -Wall -I../.. -I../ -I$(HOME)/local/include -O1 -DNDEBUG # -ggdb #-O1 -DNDEBUG 

all: table photo

tabledbg: demat.hh
	g++ -Wextra -Wall -I../.. -I../ -I. -I$(HOME)/local/include -O1 -g src/table.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/table.out

table: demat.hh
	g++ $(CXX_FLAGS) src/table.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/table.out

oldtable: demat.old.hh
	g++ $(CXX_FLAGS) src/table_old.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/table_old.out

photo: demat.hh
	g++ $(CXX_FLAGS) src/photo.cc $(HOME)/local/lib/libtesseract_full.a -lpthread -o bin/photo.out


dmap:
	g++ $(CXX_FLAGS) src/dmap.cc -o bin/dmap.out

rectangularity:
	g++ $(CXX_FLAGS) src/rectangularity.cc -o bin/rectangularity.out


extract_text_single_link:
	g++ $(CXX_FLAGS) src/extract_text_single_link.cc -o bin/extract_text_single_link.out

extract_text_double_link:
	g++ $(CXX_FLAGS) src/extract_text_double_link.cc -o bin/extract_text_double_link.out

extract_text_double_several_links:
	g++ $(CXX_FLAGS) src/extract_text_double_several_links.cc -o bin/extract_text_double_several_links.out

extract_text_multiple_links:
	g++ $(CXX_FLAGS) src/extract_text_multiple_links.cc -o bin/extract_text_multiple_links.out

extract_text_several_multiple_links:
	g++ $(CXX_FLAGS) src/extract_text_several_multiple_links.cc -o bin/extract_text_several_multiple_links.out

extract_text_several_left_links:
	g++ $(CXX_FLAGS) src/extract_text_several_left_links.cc -o bin/extract_text_several_left_links.out

table_rebuild_rank:
	g++ $(CXX_FLAGS) src/table_rebuild_rank.cc -o bin/table_rebuild_rank.out

table_rebuild_opening:
	g++ $(CXX_FLAGS) src/table_rebuild_opening.cc -o bin/table_rebuild_opening.out

table_extract:
	g++ $(CXX_FLAGS) src/table_extract.cc -o bin/table_extract.out

table_erase:
	g++ $(CXX_FLAGS) src/table_erase.cc -o bin/table_erase.out

thin_bboxes:
	g++ $(CXX_FLAGS) src/thin_bboxes.cc -o bin/thin_bboxes.out

photo_basic:
	g++ $(CXX_FLAGS) src/photo_basic.cc -o bin/photo_basic.out

clean:
	rm *.ppm *.pgm *.pbm

dist-clean: clean
	rm -f bin/table.out bin/photo.out
