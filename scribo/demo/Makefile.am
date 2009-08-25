## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

.PHONY : demo

Makefile.demo:
	qmake-qt4 $(srcdir)/demo/demo.pro -o Makefile.demo

demo: Makefile.demo
	make -f Makefile.demo all
