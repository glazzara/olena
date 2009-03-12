## Process this file through Automake to create Makefile.in -*- Makefile -*-

##FIXME
SUBDIRS = 	\
  doc 		\
  mesh 	 	\
  tests
#  apps

.PHONY: doc tutorial tools

doc:
	$(MAKE) -C doc doc

tutorial:
	$(MAKE) -C doc tutorial

tools:
	$(MAKE) -C tools all

.PHONY: regen-dist
regen-dist: $(srcdir)/headers.mk
# FIXME: Change generate_dist_headers.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
$(srcdir)/headers.mk: $(srcdir)/generate_dist_headers.sh
	cd $(srcdir) && ./generate_dist_headers.sh && chmod -w headers.mk

include $(srcdir)/headers.mk

EXTRA_DIST = 		\
img/README 		\
img/fly.pbm 		\
img/fly.pgm 		\
img/fly.ppm 		\
img/lena.pbm 		\
img/lena.pgm 		\
img/lena.ppm 		\
img/lena_16.ppm 	\
img/medium.pgm 		\
img/medium.ppm 		\
img/picasso.pbm 	\
img/picasso.pgm 	\
img/quite-small.pgm 	\
img/small-enlarged.png 	\
img/small.pbm 		\
img/small.pgm 		\
img/small.png 		\
img/small.ppm 		\
img/squares-lc.pgm 	\
img/squares.pgm 	\
img/test.fits 		\
img/tiny.pbm 		\
img/tiny.pgm 		\
img/tiny.ppm 		\
img/toto.pbm

EXTRA_DIST += 		\
generate_dist_headers.sh\
tools 			\
tools/area_flooding.cc 	\
tools/seed2tiling.cc

