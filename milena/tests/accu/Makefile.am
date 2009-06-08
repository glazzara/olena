## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  image 					\
  site_set 					\
  stat

check_PROGRAMS =				\
  all_accus					\
  bbox						\
  compute					\
  count						\
  histo						\
  line						\
  max_site 					\
  nil						\
  pair						\
  rank						\
  transform					\
  transform_diagonal				\
  transform_directional				\
  transform_line				\
  transform_snake				\
  tuple

all_accus_SOURCES = all_accus.cc
bbox_SOURCES = bbox.cc
compute_SOURCES = compute.cc
count_SOURCES = count.cc
histo_SOURCES = histo.cc
line_SOURCES = line.cc
max_site_SOURCES = max_site.cc
nil_SOURCES = nil.cc
pair_SOURCES = pair.cc
rank_SOURCES = rank.cc
transform_SOURCES = transform.cc
transform_diagonal_SOURCES = transform_diagonal.cc
transform_directional_SOURCES = transform_directional.cc
transform_line_SOURCES = transform_line.cc
transform_snake_SOURCES = transform_snake.cc
tuple_SOURCES = tuple.cc

TESTS = $(check_PROGRAMS)
