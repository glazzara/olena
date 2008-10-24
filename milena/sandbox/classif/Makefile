INCLUDES=-I../..

SRC=iccvg04.cc
TARGET=iccvg

TARGET_DBG=iccvg_dbg

IMG?=../../img/lena.ppm
DIV?=8
LAMBDA?=10

LOG=> stdout.log 2> stderr.log

all: $(TARGET)

$(TARGET): $(SRC)
	g++ $(INCLUDES) -O1 -DNDEBUG $(SRC) -o $(TARGET)

debug: $(TARGET_DBG)

iccvg_dbg: $(SRC)
	g++ $(INCLUDES) -O0 -g3 -ggdb3 $(SRC) -o $(TARGET_DBG)

.PHONY:clean check check-debug valgrind

clean:
	rm -f $(TARGET)
	rm -f $(TARGET_DBG)

check-debug: $(TARGET_DBG)
	./iccvg_dbg $(IMG) $(DIV) $(LAMBDA) $(LOG)
	cat stdout.log
	display out.ppm &

check: $(TARGET)
	./iccvg $(IMG) $(DIV) $(LAMBDA) $(LOG)
	cat stdout.log
	display out.ppm &

valgrind:
	valgrind --log-file=valgrind.log ./iccvg_dbg ../../img/lena.ppm $(DIV) $(LAMBDA) $(LOG)

gdb: $(TARGET_DBG)
	echo "run ../../img/lena.ppm $(DIV) $(LAMBDA)" > gdb.cmd
	gdb $(TARGET_DBG) -x gdb.cmd
