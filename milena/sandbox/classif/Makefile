DISP=xv

# ICCVG
ICCVG_INCLUDES=-I../..
ICCVG_SRC=iccvg04.cc
ICCVG=iccvg
ICCVG_DBG=iccvg_dbg

IMG?=../../img/lena.ppm
DIV?=8
LAMBDA?=10


# V2
V2_INCLUDES=-I../.. -I../garrigues/fllt
V2_SRC=v2.cc max_tree.hh
V2=v2
V2_DBG=v2_dbg


LOG=> stdout.log 2> stderr.log


$(ICCVG): $(ICCVG_SRC)
	g++ $(ICCVG_INCLUDES) -O1 -DNDEBUG $(ICCVG_SRC) -o $(ICCVG)


$(ICCVG_DBG): $(ICCVG_SRC)
	g++ $(ICCVG_INCLUDES) -O0 -g3 -ggdb3 $(ICCVG_SRC) -o $(ICCVG_DBG)

$(V2): $(V2_SRC)
	g++ $(V2_INCLUDES) -O1 -DNDEBUG $(V2_SRC) -o $(V2)


$(V2_DBG): $(V2_SRC)
	g++ $(V2_INCLUDES) -O0 -g3 -ggdb3 $(V2_SRC) -o $(V2_DBG)


.PHONY:clean check check-debug valgrind

clean:
	rm -f $(ICCVG)
	rm -f $(ICCVG_DBG)
	rm -f $(V2)
	rm -f $(V2_DBG)
	rm -f *.log

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
