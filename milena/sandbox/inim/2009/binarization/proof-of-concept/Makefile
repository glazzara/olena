##
## Makefile for inim
## Made by nicuveo <nicuveo@lrde.epita.fr>
##

include Makefile.rules



# VARS

PROJ = binarisation

TB = $(PROJ)
TB_TGZ     = $(TB).tgz
TB_BZ2     = $(TB).bz2
TB_TAR.GZ  = $(TB).tar.gz
TB_TAR.BZ2 = $(TB).tar.bz2

CLEAN = '*~' '\#*' '.\#*' '.DS_Store' '.gdb_history' '*.stackdump'\
'.gdb_history' 'gmon.out'



# MAIN

all:
	cd ./build/ && make all

basic:
	cd ./build/ && make basic

gradient:
	cd ./build/ && make gradient

clean: do_clean
	cd ./build/   && make -k clean
	cd ./check/   && make -k clean

distclean: do_distclean
	cd ./build/   && make -k distclean
	cd ./check/   && make -k distclean

check: all
	cd ./check/ && make check

recheck: all
	cd ./check/ && make clean check

flower:
	cat ./scripts/flower

todo:
	./scripts/todo.sh

dist tar tarball: $(TB_TAR.BZ2)



# CLEAN

do_clean:
	for f in ${CLEAN} ; do find . -name "$$f" | xargs rm -f ; done

do_distclean: do_clean
	rm -f $(TB_TGZ) $(TB_BZ2) $(TB_TAR.GZ) $(TB_TAR.BZ2)
	rm -f Makefile.rules



# TARBALLS

$(TB_TAR.GZ) $(TB_TGZ) : distclean
	./scripts/init_tar.sh "$(TB)"
	tar -hcvzf $@ --exclude=".svn" "$(TB)"
	rm -Rf "$(TB)"

$(TB_TAR.BZ2) $(TB_BZ2) : distclean
	./scripts/init_tar.sh "$(TB)"
	tar -hcvjf $@ --exclude=".svn" "$(TB)"
	rm -Rf "$(TB)"



# SPECIALS

.PHONY: todo
