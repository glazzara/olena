## Process this file through Automake to produce Makefile.in.

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux external milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

.PHONY: doc pretty-check

# Shortcut to generate the documentation from the top level.
doc:
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@

# Shortcut to launch the pretty check from the top level.
pretty-check:
	cd milena/tests && $(MAKE) $(AM_MAKEFLAGS) $@

