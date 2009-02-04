## Process this file through Automake to produce Makefile.in -*- Makefile -*-

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux external milena

.PHONY: doc
doc:
	$(MAKE) -C milena doc

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA
