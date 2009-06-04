## Process this file through Automake to produce Makefile.in.

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux external milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

# Shortcut to generate the documentation from the top level.
.PHONY: doc
doc:
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@

pretty_check:
	$(MAKE) -C milena/tests pretty_check

