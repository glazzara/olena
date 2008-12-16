## Process this file through Automake to create Makefile.in -*- Makefile -*-

##FIXME
SUBDIRS = 	\
  doc 		\
  mesh 	 	\
  tests		\
  tools
#  apps

.PHONY: doc
doc:
	$(MAKE) -C doc doc

include $(top_srcdir)/milena/headers.mk

EXTRA_DIST = \
img/lena.pbm \
img/fly.pbm \
img/squares-lc.pgm \
img/lena_16.ppm \
img/squares.pgm \
img/fly.pgm \
img/toto.pbm \
img/README \
img/quite-small.pgm \
img/test.fits \
img/lena.pgm \
img/tiny.pbm \
img/tiny.pgm \
img/picasso.pbm \
img/space_debris.pgm \
img/picasso.pgm \
img/small.pbm \
img/lena.ppm \
img/small.pgm

EXTRA_DIST += 		\
generate_dist_headers.sh
