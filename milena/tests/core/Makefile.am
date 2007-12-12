## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  category					\
  clone						\
  exact						\
  initialize					\
  mesh_elt_window				\
  mesh_image					\
  mono_obased_rle_image				\
  mono_rle_image				\
  obased_rle_image				\
  p_runs					\
  rle_image					\
  sparse_image					\
  t_image

category_SOURCES = category.cc
clone_SOURCES = clone.cc
exact_SOURCES = exact.cc
initialize_SOURCES = initialize.cc
mesh_elt_window_SOURCES = mesh_elt_window.cc
mesh_image_SOURCES = mesh_image.cc
mono_obased_rle_image_SOURCES = mono_obased_rle_image.cc
mono_rle_image_SOURCES = mono_rle_image.cc
obased_rle_image_SOURCES = obased_rle_image.cc
p_runs_SOURCES = p_runs.cc
rle_image_SOURCES = rle_image.cc
sparse_image_SOURCES = sparse_image.cc
t_image_SOURCES = t_image.cc

TESTS = $(check_PROGRAMS)
