# Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
# Laboratory (LRDE).
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

CLEANFILES =
AM_CPPFLAGS = -I$(PYTHONINC) -I$(top_srcdir)/milena
TOOLS_CXXFLAGS = @TOOLS_CXXFLAGS@
AM_CXXFLAGS = $(TOOLS_CXXFLAGS)
AM_SWIGFLAGS = -Wall -c++ -python -I$(top_srcdir)/milena
# We build modules, not plain libs.
AM_LDFLAGS = -avoid-version -module -shared

# Run Swig to create the C++ wrapper files, the Python interface
# files, and the dependency Makefile snippets.
%-wrap.cc %.py: $(top_srcdir)/swilena/%.i
@AMDEP_TRUE@	if $(SWIG) $(AM_SWIGFLAGS) $(SWIGFLAGS) -MD -MF "$(DEPDIR)/$*-wrap.Tcc" -o $@ $<; then \
@AMDEP_TRUE@	  mv -f "$(DEPDIR)/$*-wrap.Tcc" "$(DEPDIR)/$*-wrap.Pcc";\
@AMDEP_TRUE@	else \
@AMDEP_TRUE@	  rm -f "$(DEPDIR)/$*-wrap.Tcc"; exit 1;\
@AMDEP_TRUE@	fi
@AMDEP_FALSE@	$(SWIG) $(AM_SWIGFLAGS) $(SWIGFLAGS) -o $@ $<


# ltihooks.py: Python import hooks that understand Libtool libraries.
python_PYTHON = ltihooks.py
# swilena.py: The whole Swilena suite.
python_PYTHON += swilena.py

# data.py: Access to the data of the distribution (images, meshes, etc.)
EXTRA_DIST = data.py


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

## config.
pyexec_LTLIBRARIES += _config.la
nodist__config_la_SOURCES = config-wrap.cc
_config_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__config_la_SOURCES) config.py config.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_config-wrap.Pcc@am__quote@
nodist_python_PYTHON += config.py

## point2d.
pyexec_LTLIBRARIES += _point2d.la
nodist__point2d_la_SOURCES = point2d-wrap.cc
_point2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__point2d_la_SOURCES) point2d.py point2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_point2d-wrap.Pcc@am__quote@
nodist_python_PYTHON += point2d.py

## dpoint2d.
pyexec_LTLIBRARIES += _dpoint2d.la
nodist__dpoint2d_la_SOURCES = dpoint2d-wrap.cc
_dpoint2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__dpoint2d_la_SOURCES) dpoint2d.py dpoint2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_dpoint2d-wrap.Pcc@am__quote@
nodist_python_PYTHON += dpoint2d.py

## box2d.
pyexec_LTLIBRARIES += _box2d.la
nodist__box2d_la_SOURCES = box2d-wrap.cc
_box2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__box2d_la_SOURCES) box2d.py box2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_box2d-wrap.Pcc@am__quote@
nodist_python_PYTHON += box2d.py

## box2d_piter.
pyexec_LTLIBRARIES += _box2d_piter.la
nodist__box2d_piter_la_SOURCES = box2d_piter-wrap.cc
_box2d_piter_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__box2d_piter_la_SOURCES) box2d_piter.py box2d_piter.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_box2d_piter-wrap.Pcc@am__quote@
nodist_python_PYTHON += box2d_piter.py

## int_u8.
pyexec_LTLIBRARIES += _int_u8.la
nodist__int_u8_la_SOURCES = int_u8-wrap.cc
_int_u8_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__int_u8_la_SOURCES) int_u8.py int_u8.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_int_u8-wrap.Pcc@am__quote@
nodist_python_PYTHON += int_u8.py

## int_u24.
pyexec_LTLIBRARIES += _int_u24.la
nodist__int_u24_la_SOURCES = int_u24-wrap.cc
_int_u24_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__int_u24_la_SOURCES) int_u24.py int_u24.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_int_u24-wrap.Pcc@am__quote@
nodist_python_PYTHON += int_u24.py

## neighb2d.
pyexec_LTLIBRARIES += _neighb2d.la
nodist__neighb2d_la_SOURCES = neighb2d-wrap.cc
_neighb2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__neighb2d_la_SOURCES) neighb2d.py neighb2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_neighb2d-wrap.Pcc@am__quote@
nodist_python_PYTHON += neighb2d.py

## window2d.
pyexec_LTLIBRARIES += _window2d.la
nodist__window2d_la_SOURCES = window2d-wrap.cc
_window2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__window2d_la_SOURCES) window2d.py window2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_window2d-wrap.Pcc@am__quote@
nodist_python_PYTHON += window2d.py

## image2d_int.
pyexec_LTLIBRARIES += _image2d_int.la
nodist__image2d_int_la_SOURCES = image2d_int-wrap.cc
_image2d_int_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__image2d_int_la_SOURCES) image2d_int.py image2d_int.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_image2d_int-wrap.Pcc@am__quote@
nodist_python_PYTHON += image2d_int.py

## image2d_int_u8.
pyexec_LTLIBRARIES += _image2d_int_u8.la
nodist__image2d_int_u8_la_SOURCES = image2d_int_u8-wrap.cc
_image2d_int_u8_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__image2d_int_u8_la_SOURCES) image2d_int_u8.py image2d_int_u8.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_image2d_int_u8-wrap.Pcc@am__quote@
nodist_python_PYTHON += image2d_int_u8.py

## dynamic_image2d.
pyexec_LTLIBRARIES += _dynamic_image2d.la
# Swig also generates a header for this module, since it makes use of
# directors.
nodist__dynamic_image2d_la_SOURCES =		\
  dynamic_image2d-wrap.h			\
  dynamic_image2d-wrap.cc
_dynamic_image2d_la_LIBADD = $(AM_LIBADD)
CLEANFILES += $(nodist__dynamic_image2d_la_SOURCES) dynamic_image2d.py dynamic_image2d.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_dynamic_image2d-wrap.Pcc@am__quote@
nodist_python_PYTHON += dynamic_image2d.py


## -------------------- ##
## Extra dependencies.  ##
## -------------------- ##

# FIXME: It would be cool to have them generated automatically.

# Dependencies pulled by the meta wrapper (.ixx) files.
int_u_ixx_deps = $(top_srcdir)/swilena/python-utils.ixx

box_ixx_deps = $(top_srcdir)/swilena/coord.ixx

morpho_ixx_deps =				\
  $(top_srcdir)/swilena/concrete.ixx		\
  $(top_srcdir)/swilena/ch_value.ixx

# Dependencies of wrappers not automatically computed (yet).
int_u8-wrap.cc: $(top_srcdir)/swilena/int_u.ixx $(int_u_ixx_deps)
int_u32-wrap.cc: $(top_srcdir)/swilena/int_u.ixx $(int_u_ixx_deps)

box2d-wrap.cc: $(top_srcdir)/swilena/box.ixx $(box_ixx_deps)
box2d_piter-wrap.cc: $(top_srcdir)/swilena/box_piter.ixx

point2d-wrap.cc: $(top_srcdir)/swilena/concat.ixx
point2d-wrap.cc: $(top_srcdir)/swilena/python-utils.ixx

dynamic_image2d-wrap.cc: $(top_srcdir)/swilena/fill.ixx
dynamic_image2d-wrap.cc: $(top_srcdir)/swilena/println.ixx

image2d_int-wrap.cc: $(top_srcdir)/swilena/image2d.ixx
image2d_int-wrap.cc: $(top_srcdir)/swilena/fill.ixx
image2d_int-wrap.cc: $(top_srcdir)/swilena/println.ixx
image2d_int-wrap.cc: $(top_srcdir)/swilena/morpho.ixx $(morpho_ixx_deps)

image2d_int_u8-wrap.cc: $(top_srcdir)/swilena/image2d.ixx
image2d_int_u8-wrap.cc: $(top_srcdir)/swilena/pgm.ixx
image2d_int_u8-wrap.cc: $(top_srcdir)/swilena/fill.ixx
image2d_int_u8-wrap.cc: $(top_srcdir)/swilena/println.ixx
image2d_int_u8-wrap.cc: $(top_srcdir)/swilena/morpho.ixx $(morpho_ixx_deps)


## ---------------------------- ##
## Swilena Python Shell (sps).  ##
## ---------------------------- ##

# FIXME: We might want to rename this shell as something else.
python_PYTHON += sps.py

# The script to be used in installed distributions.
bin_SCRIPTS = sps
# The script to be used in non-installed build trees.
noinst_SCRIPTS = sps-local

CLEANFILES += $(bin_SCRIPTS) $(noinst_SCRIPTS)


## ------- ##
## Tests.  ##
## ------- ##

include $(top_srcdir)/swilena/run.mk

## FIXME: Do we really need to pass top_srcdir and top_builddir to run?
TESTS_ENVIRONMENT = \
  top_srcdir="$(top_srcdir)" top_builddir="$(top_builddir)" $(RUN)
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
TESTS =									\
  box2d-misc.py								\
  image2d-misc.py morpho-fun.py morpho-segm.py dynamic-image2d-misc.py

EXTRA_DIST += $(TESTS)

# Images generated by tests.
MOSTLYCLEANFILES =				\
  morpho-fun-closing.pgm			\
  morpho-fun-closing_area.pgm			\
  morpho-fun-dilation.pgm			\
  morpho-fun-erosion.pgm			\
  morpho-fun-gradient.pgm			\
  morpho-fun-gradient_external.pgm		\
  morpho-fun-gradient_internal.pgm		\
  morpho-fun-opening.pgm			\
  morpho-fun-opening_area.pgm			\
  morpho-segm-gradient.pgm			\
  morpho-segm-ws.pgm
