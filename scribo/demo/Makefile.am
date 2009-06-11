## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

.PHONY : demo

Makefile.demo:
	qmake-qt4 $(top_srcdir)/scribo/demo/demo.pro -o Makefile.demo

demo: Makefile.demo
	make -f Makefile.demo all
