## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = approx naive

check_PROGRAMS =				\
  abs						\
  all_headers					\
  apply						\
  compare					\
  compute					\
  convert					\
  median					\
  median_fast					\
  saturate					\
  sort_psites					\
  stretch					\
  transform					\
  transform_inplace				\
  update

abs_SOURCES = abs.cc
all_headers_SOURCES = all_headers.cc
apply_SOURCES = apply.cc
compare_SOURCES = compare.cc
compute_SOURCES = compute.cc
convert_SOURCES = convert.cc
median_SOURCES = median.cc
median_fast_SOURCES = median_fast.cc
saturate_SOURCES = saturate.cc
sort_psites_SOURCES = sort_psites.cc
stretch_SOURCES = stretch.cc
transform_SOURCES = transform.cc
transform_inplace_SOURCES = transform_inplace.cc
update_SOURCES = update.cc

TESTS = $(check_PROGRAMS)
