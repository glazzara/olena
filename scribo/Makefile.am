## Process this file through Automake to create Makefile.in.

SUBDIRS = tests src

.PHONY: bin

bin:
	make -C src

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += tests
