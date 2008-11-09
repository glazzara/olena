DISP=xv

# ICCVG
ICCVG_INCLUDES=-I../../..
ICCVG_SRC=src/iccvg04.cc
ICCVG=iccvg
ICCVG_DBG=iccvg_dbg

IMG?=../../img/lena.ppm
DIV?=4
LAMBDA?=10

# V2
V2_INCLUDES=-I../../..
V2_SRC=src/v2.cc src/max_tree.hh
V2=v2
V2_DBG=v2_dbg


LOG=> stdout.log 2> stderr.log


$(ICCVG): $(ICCVG_SRC)
	g++ $(ICCVG_INCLUDES) -Wall -W -O1 -DNDEBUG $(ICCVG_SRC) -o $(ICCVG)


$(ICCVG_DBG): $(ICCVG_SRC)
	g++ $(ICCVG_INCLUDES) -Wall -W -O0 -g3 -ggdb3 $(ICCVG_SRC) -o $(ICCVG_DBG)

$(V2): $(V2_SRC)
	g++ $(V2_INCLUDES) -Wall -W -O1 -DNDEBUG $(V2_SRC) -o $(V2)


$(V2_DBG): $(V2_SRC) .dep_v2
	g++ $(V2_INCLUDES) -Wall -W -O0 -g3 -ggdb3 $(V2_SRC) -o $(V2_DBG)


.PHONY:clean check check-debug valgrind .dep_iccvg .dep_v2

clean:
	rm -f $(ICCVG)
	rm -f $(ICCVG_DBG)
	rm -f $(V2)
	rm -f $(V2_DBG)
	rm -f *.log
	rm -f .dep_iccvg .dep_v2 .dep_iccvgr .dep_v2r

check-debug: $(ICCVG_DBG)
	./iccvg_dbg $(IMG) $(DIV) $(LAMBDA) $(LOG)
	cat stdout.log
	$(DISP) out.ppm &

check: $(ICCVG)
	./iccvg $(IMG) $(DIV) $(LAMBDA) $(LOG)
	cat stdout.log
	$(DISP) out.ppm &

v2-check: $(V2)
	./v2 $(IMG) $(DIV) $(LAMBDA) $(LOG)
	cat stdout.log
	$(DISP) out.ppm &

valgrind: $(ICCVG_DBG)
	valgrind --log-file=valgrind.log ./iccvg_dbg ../../img/lena.ppm $(DIV) $(LAMBDA) $(LOG)

gdb: $(ICCVG_DBG)
	echo "run ../../img/lena.ppm $(DIV) $(LAMBDA)" > gdb.cmd
	gdb $(ICCVG_DBG) -x gdb.cmd

v2-gdb: $(V2_DBG)
	echo "run ../../img/lena.ppm $(DIV) $(LAMBDA) $(LOG)" > gdb.cmd
	gdb $(V2_DBG) -x gdb.cmd


.dep_iccvg:
	g++ $(ICCVG_INCLUDES) -MM src/iccvg04.cc > $@
	@sed -ir s/iccvg04.cc// .dep_iccvg
	@sed -ir s/iccvg04.o/iccvg/ .dep_iccvg

.dep_v2:
	g++ $(V2_INCLUDES) -MM src/v2.cc > $@
	@sed -ir s/v2.cc// .dep_v2
	@sed -ir s/v2.o/v2/ .dep_v2

-include .dep_iccvg
-include .dep_v2
