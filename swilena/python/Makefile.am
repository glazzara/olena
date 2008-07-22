## Process this file through Automake to produce Makefile.in -*- Makefile -*-

## ------------------ ##
## Generic material.  ##
## ------------------ ##

## FIXME: Factor as much as possible.  See how we handled this in TC.

CLEANFILES =
AM_CPPFLAGS = -I$(PYTHONINC) -I$(top_srcdir)/milena
AM_SWIGFLAGS = -c++ -python -I$(top_srcdir)/milena
## We build modules, not plain libs.
AM_LDFLAGS = -avoid-version -module -shared
#### All the modules depend on libhw.
##AM_LIBADD = $(top_builddir)/libhw.la

## Run Swig to create the C++ wrapper files, the Python interface
## files, and the dependency Makefile snippets.
%-wrap.cc %.py: $(top_srcdir)/swilena/%.i
	if $(SWIG) $(AM_SWIGFLAGS) $(SWIGFLAGS) -MD -MF "$(DEPDIR)/$*-wrap.Tcc" -o $@ $<; then \
	  mv -f "$(DEPDIR)/$*-wrap.Tcc" "$(DEPDIR)/$*-wrap.Pcc";\
	else \
	  rm -f "$(DEPDIR)/$*-wrap.Tcc"; exit 1;\
	fi


# ltihooks.py: Python import hooks that understand Libtool libraries.
python_PYTHON = ltihooks.py
# swilena.py: The whole Swilena suite.
python_PYTHON += swilena.py


## ----------------- ##
## Wrapped modules.  ##
## ----------------- ##

## FIXME: All of this should be generated.
## Don't forget to add a `deps-reset' target as in TC.

## image2d.
pyexec_LTLIBRARIES = _image2d.la
nodist__image2d_la_SOURCES = image2d-wrap.cc
_image2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__image2d_la_SOURCES) image2d.py image2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_image2d-wrap.Pcc@am__quote@

nodist_python_PYTHON = image2d.py


## ---------------------------- ##
## Swilena Python Shell (sps).  ##
## ---------------------------- ##

# FIXME: We might want to rename this shell as something else.
python_PYTHON += sps.py

# The script to be used in installed distributions.
bin_SCRIPTS = sps
# The script to be used in non-installed distributions.
noinst_SCRIPTS = sps-local

CLEANFILES += $(bin_SCRIPTS) $(noinst_SCRIPTS)


## ------- ##
## Tests.  ##
## ------- ##

include $(top_srcdir)/swilena/run.mk

## FIXME: Do we really need to pass top_srcdir and top_builddir to run?
##TESTS_ENVIRONMENT = \
##  top_srcdir="$(top_srcdir)" top_builddir="$(top_builddir)" $(RUN)
TESTS_ENVIRONMENT = $(RUN)
# Ensure `run' is rebuilt before the tests are run.
$(TESTS): $(srcdir)/run.stamp
# The dependency is on `run.in' and not `run', since `run' is
# regenerated at distribution time, and voids the time stamps (which
# we don't want!).
EXTRA_DIST = $(srcdir)/run.stamp
$(srcdir)/run.stamp: $(RUN_IN)
	@rm -f $@
	@rm -f $@.tmp
	@touch $@.tmp
	$(MAKE) $(AM_MAKEFLAGS) $(RUN)
	@mv -f $@.tmp $@

TESTS = test.py
EXTRA_DIST += $(TESTS)
