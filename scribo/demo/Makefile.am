## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

Makefile.demo:
	qmake-qt4 $(top_srcdir)/scribo/src/demo/demo.pro -o Makefile.demo

demo: Makefile.demo
	make -f Makefile.demo all
