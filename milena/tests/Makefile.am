## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

.PHONY: pretty-check

SUBDIRS =					\
  accu						\
  algebra					\
  arith						\
  binarization					\
  border					\
  canvas					\
  convert					\
  core						\
  data						\
  debug						\
  display					\
  draw						\
  estim						\
  extension					\
  fun						\
  geom						\
  graph 					\
  histo						\
  io						\
  labeling					\
  linear					\
  literal					\
  logical					\
  make						\
  math						\
  metal						\
  morpho					\
  norm						\
  opt						\
  pw						\
  registration 					\
  set						\
  subsampling					\
  tag						\
  test						\
  topo						\
  trace						\
  trait						\
  transform 					\
  transformation				\
  unit_test					\
  util						\
  value						\
  win 						\
  world


check_PROGRAMS =				\
  all_headers 					\
  essential_headers 				\
  global_vars

all_headers_SOURCES = all_headers.cc
essential_headers_SOURCES = essential_headers.cc
global_vars_SOURCES = global_vars1.cc global_vars2.cc

TESTS = $(check_PROGRAMS)

pretty-check:
	$(top_srcdir)/milena/tests/tools/pretty_check.sh


EXTRA_DIST = 					\
  img/lena_ascii.pbm 				\
  img/lena_ascii.pgm				\
  img/lena_raw.pbm				\
  img/lena_raw.pgm				\
  img/test_bw.tif				\
  img/test_gl.tif				\
  img/test_rgb8.tif				\
  img/tiny.png

