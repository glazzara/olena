CXXFLAGS=-I../../.. -I${HOME}/local/include
LFLAGS=-L${HOME}/local/lib -ltesseract_full -lpthread

all: ocr_without_preprocess ocr_with_preprocess

ocr_without_preprocess: ocr_without_preprocess.cc
	 g++ -DNDEBUG -O3 ${CXXFLAGS} $< ${LFLAGS} -o $@

ocr_with_preprocess: ocr_with_preprocess.cc
	 g++ -DNDEBUG -O3 ${CXXFLAGS} $< ${LFLAGS} -o $@
clean:
	rm -f *.o ocr_without_preprocess ocr_with_preprocess

logs:
	mkdir logs

tmp:
	mkdir tmp

check: logs tmp ocr_without_preprocess ocr_with_preprocess
	./check.sh
