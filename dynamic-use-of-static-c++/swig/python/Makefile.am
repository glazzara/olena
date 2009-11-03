# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.


## ------------------ ##
## Generic material.  ##
## ------------------ ##

## FIXME: Factor as much as possible.  See how we handled this in TC.

## FIXME: Adjust.

CLEANFILES =
AM_CPPFLAGS = -I$(PYTHONINC) -I$(top_builddir)/src -I$(top_srcdir)/src
## FIXME: Define an equivalent to Olena's TOOLS_CXXFLAGS?
##TOOLS_CXXFLAGS = @TOOLS_CXXFLAGS@
##AM_CXXFLAGS = $(TOOLS_CXXFLAGS)
## FIXME: Warning, `AM_SWIGFLAGS' is not a standard Automake variable.
AM_SWIGFLAGS = -Wall -c++ -python -I$(top_builddir)/src -I$(top_srcdir)/src
# We build modules, not plain libs.
AM_LDFLAGS = -avoid-version -module -shared
# Libraries with their dependencies.
libdyn_la = $(top_builddir)/src/libdyn.la
libdynmilena_la = $(top_builddir)/src/wrappers/libdynmilena.la $(libdyn_la)

# Run Swig to create the C++ wrapper files, the Python interface
# files, and the dependency Makefile snippets.
# FIXME: `%'-style patterns rules are not portable.
%-wrap.cc %.py: $(top_srcdir)/swig/%.i
	if $(SWIG) $(AM_SWIGFLAGS) $(SWIGFLAGS) -MD -MF "$(DEPDIR)/$*-wrap.Tcc" -o $@ $<; then \
	  mv -f "$(DEPDIR)/$*-wrap.Tcc" "$(DEPDIR)/$*-wrap.Pcc";\
	else \
	  rm -f "$(DEPDIR)/$*-wrap.Tcc"; exit 1;\
	fi


# ltihooks.py: Python import hooks that understand Libtool libraries.
python_PYTHON = ltihooks.py

# config.py: Configuration values of the package.
EXTRA_DIST = config.py


## ----------------- ##
## Wrapped modules.  ##
## ----------------- ##

## FIXME: All of this should be generated.
## Don't forget to add a `deps-reset' target as in TC.

## FIXME: Dependencies do not work as expected, e.g., touching
## mln/core/point.hh will not void point-wrap.cc, and Swig will not
## regen it (it will merely recompile _point2d.la).  See how other
## projects people handle this.

nodist_python_PYTHON =
pyexec_LTLIBRARIES =

## dyn.
pyexec_LTLIBRARIES += _dyn.la
nodist__dyn_la_SOURCES = dyn-wrap.cc
_dyn_la_LIBADD = $(libdyn_la)
CLEANFILES += $(nodist__dyn_la_SOURCES) dyn.py dyn.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_dyn-wrap.Pcc@am__quote@
nodist_python_PYTHON += dyn.py

# FIXME: Should be optional.
## mln.
pyexec_LTLIBRARIES += _mln.la
nodist__mln_la_SOURCES = mln-wrap.cc
_mln_la_LIBADD = $(libdynmilena_la)
CLEANFILES += $(nodist__mln_la_SOURCES) mln.py mln.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_mln-wrap.Pcc@am__quote@
nodist_python_PYTHON += mln.py


## ------- ##
## Tests.  ##
## ------- ##

include $(top_srcdir)/swig/run.mk

TESTS_ENVIRONMENT =				\
  abs_top_srcdir="$(abs_top_srcdir)"		\
  abs_top_builddir="$(abs_top_builddir)"	\
  $(RUN)
# Ensure `run' is rebuilt before the tests are run.
$(TESTS): $(srcdir)/run.stamp
# The dependency is on `run.in' and not `run', since `run' is
# regenerated at distribution time, and voids the time stamps (which
# we don't want!).
EXTRA_DIST += $(srcdir)/run.stamp
$(srcdir)/run.stamp: $(RUN_IN)
	@rm -f $@
	@rm -f $@.tmp
	@touch $@.tmp
	$(MAKE) $(AM_MAKEFLAGS) $(RUN)
	@mv -f $@.tmp $@

# FIXME: We should not use the `.py' extension: it is not needed, and
# it prevents us from using the same name for both a module (wrapper)
# and a test.  Alas, the script `run' expects a file name with an
# extension as argument.  We could improve this by adding options such
# as `--python' to `run'.
TESTS = milena-libdyn.py milena-libdynmilena.py

# FIXME: Is this really needed?
EXTRA_DIST += $(TESTS)

clean-local: clean-repository
.PHONY: clean-repository
clean-repository:
	-rm -rf repository
