### Makefile --- 

## Author: ugo.jardonnet@lrde.epita.fr
## Version: $Id: Makefile,v 0.0 2008/11/19 11:40:34 jardonnet Exp $
## Keywords: 
## X-URL: 

PROJ=igr-reco
SRC=src/igr.cc
BIN=igr
LFLAGS=-I../../abraham/ -I../../../
CFLAGS=-W -Wall -Wextra -O1 -DNDEBUG
FILES=src configure check README

$(BIN): Makefile
	g++ $(CFLAGS) $(LFLAGS) $(SRC) -o $(BIN)

check:
	cd check && ./check

clean:
	rm -f $(BIN)
	rm -f .deps .depsr
	rm -f *.ppm

dist: clean todo AUTHORS svn_check
	rm -rf $(PROJ)
	mkdir $(PROJ)
	cp -r Makefile AUTHORS TODO $(FILES) $(PROJ)
	-find $(PROJ) -name ".svn" -exec rm -rf {} \;
	-find $(PROJ) -name "*~" -exec rm -rf {} \;
	-find $(PROJ) -name "#*" -exec rm -rf {} \;
	tar cjvf $(PROJ).tar.bz2 $(PROJ)
	rm -rf $(PROJ)
	chmod 644 $(PROJ).tar.bz2

svn_check:
	@svn st | grep \? ; [ $$? = "1" ] \
	|| (echo "----------------------------------------------"\
	&& echo "SOME FILES ARE MISSING FROM THE SVN REPOSITORY"\
	&& echo "----------------------------------------------");

todo:
	grep "FIXME" -r . --exclude="Makefile" > TODO

.deps:
	g++ $(LFLAGS) -MM $(SRC) > $@
	@sed -ir s/igr.cc// .deps
	@sed -ir s/igr.o/igr/ .deps

.PHONY: doc check .deps

include .deps

### Makefile ends here