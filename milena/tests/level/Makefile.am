## Process this file through Automake to create Makefile.in -*- Makefile -*-

SUBDIRS = approx naive

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  abs						\
  all_headers					\
  apply						\
  assign					\
  compare					\
  compute					\
  fill						\
  median					\
  median_dir					\
  median_fast					\
  median_hline2d				\
  memcpy_					\
  memset_					\
  paste						\
  saturate					\
  sort_psites					\
  sort_psites_full				\
  stretch					\
  take						\
  transform

abs_SOURCES = abs.cc
all_headers_SOURCES = all_headers.cc
apply_SOURCES = apply.cc
assign_SOURCES = assign.cc
compare_SOURCES = compare.cc
compute_SOURCES = compute.cc
fill_SOURCES = fill.cc
median_SOURCES = median.cc
median_dir_SOURCES = median_dir.cc
median_fast_SOURCES = median_fast.cc
median_hline2d_SOURCES = median_hline2d.cc
memcpy__SOURCES = memcpy_.cc
memset__SOURCES = memset_.cc
paste_SOURCES = paste.cc
saturate_SOURCES = saturate.cc
sort_psites_SOURCES = sort_psites.cc
stretch_SOURCES = stretch.cc
take_SOURCES = take.cc
transform_SOURCES = transform.cc

# Lengthy tests.
sort_psites_full_SOURCES = sort_psites_full.cc

TESTS = $(check_PROGRAMS)
