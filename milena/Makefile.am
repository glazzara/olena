## Process this file through Automake to create Makefile.in.

SUBDIRS = 	\
  doc 		\
  mesh 	 	\
  tests

if ENABLE_APPS
  SUBDIRS += apps
endif ENABLE_APPS

if ENABLE_TOOLS
  SUBDIRS += tools
endif ENABLE_TOOLS

.PHONY: doc tutorial tools

doc:
	$(MAKE) -C doc doc

tutorial:
	$(MAKE) -C doc tutorial

tools:
	$(MAKE) -C tools all

# Force the regeneration of `headers.mk'.
.PHONY: regen-dist
regen-dist:
	cd $(srcdir) && ./generate_dist_headers.sh
EXTRA_DIST = generate_dist_headers.sh
# We do not delegate the action of regenerating `headers.mk' to
# `regen-dist' like this:
#
#   $(MAKE) $(AM_MAKEFLAGS) regen-dist
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
# Hence the redundant action.  There may be a better solution.

# FIXME: Change generate_dist_headers.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
$(srcdir)/headers.mk: $(srcdir)/generate_dist_headers.sh
	cd $(srcdir) && ./generate_dist_headers.sh

include $(srcdir)/headers.mk

EXTRA_DIST +=					\
  img/README					\
  img/fly.pbm					\
  img/fly.pgm					\
  img/fly.ppm					\
  img/lena.pbm					\
  img/lena.pgm					\
  img/lena.ppm					\
  img/lena_16.ppm				\
  img/medium.pgm				\
  img/medium.ppm				\
  img/picasso.pbm				\
  img/picasso.pgm				\
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
