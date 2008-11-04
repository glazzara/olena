## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  accu						\
  algebra					\
  arith						\
  binarization					\
  border					\
  canvas					\
  convert					\
  core						\
  debug						\
  display					\
  draw						\
  estim						\
  extension					\
  fun						\
  geom						\
  histo						\
  io						\
  labeling					\
  level						\
  linear					\
  literal					\
  logical					\
  make						\
  math						\
  metal						\
  morpho					\
  neighb					\
  norm						\
  pw						\
  set						\
  subsampling					\
  tag						\
  test						\
  topo						\
  trace						\
  trait						\
  unit_test					\
  util						\
  value						\
  win


check_PROGRAMS =				\
  all_headers 					\
  essential_headers

all_headers_SOURCES = all_headers.cc
essential_headers_SOURCES = essential_headers.cc

TESTS = $(check_PROGRAMS)
