# Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and Development
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

SUBDIRS = . doc mesh tests

if ENABLE_APPS
  SUBDIRS += apps
endif ENABLE_APPS

if ENABLE_TOOLS
  SUBDIRS += tools
endif ENABLE_TOOLS

.PHONY: pretty-check tests

pretty-check:
	cd tests && $(MAKE) $(AM_MAKEFLAGS) $@

tests:
	cd tests && $(MAKE) $(AM_MAKEFLAGS) $@

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += doc tests

# Force the regeneration of `headers.mk'.
#
# We do not delegate the action of regenerating `headers.mk' to
# `regen' like this:
#
#   $(MAKE) $(AM_MAKEFLAGS) regen
#
# to avoid an infinite recursion, since
#
# - the generation of `headers.mk' would trigger the update
#   (regeneration) of `Makefile' (due to the behavior of Automade
#   Makefiles);
#
# and because
#
# - `Makefile' depends on `headers.mk' (because of the `include'
#    statement); and
# - `headers.mk' depends on `generate_dist_headers.sh'.
#
# Hence the redundant action.  There may be a better solution
regen-am:
	cd $(srcdir)							\
	  && $(abs_top_srcdir)/build-aux/generate_dist_headers.sh	\
	       mln headers.mk
# Also regenerate mln/version.hh.  Again, the action is duplicated
# (see target $(srcdir)/mln/version.hh below).  There's probably a
# (portable) way to factor this.
	rm -f $(srcdir)/mln/version.hh $(srcdir)/mln/version.hh.tmp
	srcdir=''; \
	  test -f ./$(srcdir)/mln/version.hh.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$(srcdir)/mln/version.hh.in \
	    >$(srcdir)/mln/version.hh.tmp
	chmod a-w $(srcdir)/mln/version.hh.tmp
	mv $(srcdir)/mln/version.hh.tmp $(srcdir)/mln/version.hh

# FIXME: Change generate_dist_headers.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
$(srcdir)/headers.mk: $(top_srcdir)/build-aux/generate_dist_headers.sh
	cd $(srcdir)							\
	  && $(abs_top_srcdir)/build-aux/generate_dist_headers.sh	\
	       mln headers.mk

include $(srcdir)/headers.mk


# Package metadata (mln/version.hh).
#
# Note that contrary to $(top_srcdir)/config.h, mln/version.hh is
# meant to be installed and is part of the library.

edit = sed -e 's,@PACKAGE_BUGREPORT[@],$(PACKAGE_BUGREPORT),g'	\
           -e 's,@PACKAGE_NAME[@],$(PACKAGE_NAME),g'		\
           -e 's,@PACKAGE_STRING[@],$(PACKAGE_STRING),g'	\
           -e 's,@PACKAGE_VERSION[@],$(PACKAGE_VERSION),g'	\
           -e 's,@PACKAGE_URL[@],$(PACKAGE_URL),g'		\
           -e 's,@PACKAGE_AUTHORS[@],$(PACKAGE_AUTHORS),g'

EXTRA_DIST = mln/version.hh.in
$(srcdir)/mln/version.hh: mln/version.hh.in $(top_srcdir)/configure.ac Makefile.am
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@


imgdir = $(pkgdatadir)/images
dist_img_DATA =					\
  img/README					\
  img/bunny.pgm					\
  img/fly.pbm					\
  img/fly.pgm					\
  img/fly.ppm					\
  img/fly_ascii.ppm				\
  img/lena.pbm					\
  img/lena.pgm					\
  img/lena.ppm					\
  img/lena_16.ppm				\
  img/medium.pgm				\
  img/medium.ppm				\
  img/picasso.pbm				\
  img/picasso.pgm				\
  img/picasso.ppm				\
  img/quite-small.pgm				\
  img/small.pbm					\
  img/small.pgm					\
  img/small.ppm					\
  img/squares-lc.pgm				\
  img/squares.pgm				\
  img/test.fits					\
  img/tiny.pbm					\
  img/tiny.pgm					\
  img/tiny.ppm					\
  img/toto.pbm

EXTRA_DIST +=					\
  tests/img/test_rgb8.tif			\
  tests/img/test_bw.tif				\
  tests/img/test_gl.tif

# Header file generator.
EXTRA_DIST += new-header
