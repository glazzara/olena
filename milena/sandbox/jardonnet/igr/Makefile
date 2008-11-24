### Makefile --- 

## Author: ugo.jardonnet@lrde.epita.fr
## Version: $Id: Makefile,v 0.0 2008/11/19 11:40:34 jardonnet Exp $
## Keywords: IGR OLENA LRDE RECONSTRUCTION


CXXC?=g++
LFLAGS=-I../../abraham/ -I../../../
CFLAGS=-W -Wall -Wextra -O1 -DNDEBUG

PROJ=igr-reco
SRC=src/igr.cc
BIN=igr
FILES=src configure check README

$(BIN): Makefile
	$(CXXC) $(CFLAGS) $(LFLAGS) $(SRC) -o $(BIN)

check:
	cd check && ./check

clean:
	rm -f $(BIN)
	rm -f .deps .depsr
	rm -f *.ppm

dist: clean AUTHORS TODO svn_check
	rm -rf $(PROJ)
	mkdir $(PROJ)
	cp -r Makefile AUTHORS TODO $(FILES) $(PROJ)
	-find $(PROJ) -name ".svn" -exec rm -rf {} \;
	-find $(PROJ) -name "*~" -exec rm -rf {} \;
	-find $(PROJ) -name "#*" -exec rm -rf {} \;
	tar cjvf $(PROJ).tar.bz2 $(PROJ)
	rm -rf $(PROJ)
	chmod 644 $(PROJ).tar.bz2

AUTHORS:
	echo $(USER) > AUTHORS

svn_check:
	@svn st | grep \? ; [ $$? = "1" ] \
	|| (echo "----------------------------------------------"\
	&& echo "SOME FILES ARE MISSING FROM THE SVN REPOSITORY"\
	&& echo "----------------------------------------------");

TODO:
	grep "FIXME" -r . --exclude="Makefile" --exclude-dir=".svn" \
	--exclude="TODO" > TODO

.deps:
	g++ $(LFLAGS) -MM $(SRC) > $@
	@sed -ir s/igr.cc// .deps
	@sed -ir s/igr.o/igr/ .deps

.PHONY: doc check .deps AUTHORS TODO

include .deps

### Makefile ends here