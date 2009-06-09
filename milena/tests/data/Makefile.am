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
  fill						\
  fill_with_image				\
  fill_with_value				\
  median					\
  median_fast					\
  memcpy_					\
  memset_					\
  paste						\
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
fill_SOURCES = fill.cc
fill_with_image_SOURCES = fill_with_image.cc
fill_with_value_SOURCES = fill_with_value.cc
median_SOURCES = median.cc
median_fast_SOURCES = median_fast.cc
memcpy__SOURCES = memcpy_.cc
memset__SOURCES = memset_.cc
paste_SOURCES = paste.cc
saturate_SOURCES = saturate.cc
sort_psites_SOURCES = sort_psites.cc
stretch_SOURCES = stretch.cc
transform_SOURCES = transform.cc
transform_inplace_SOURCES = transform_inplace.cc
update_SOURCES = update.cc

# Lengthy tests.
# FIXME: enable it when make full-check is enabled.
#fill_full_SOURCES = fill_full.cc

TESTS = $(check_PROGRAMS)

XFAIL_TESTS = transform
