## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  category					\
  clone						\
  exact						\
  h_vec						\
  initialize					\
  mesh_elt_window				\
  mesh_image					\
  mono_obased_rle_image				\
  mono_rle_image				\
  obased_rle_image				\
  p_runs					\
  rle_image					\
  sparse_image					\
  t_image					\
  tr_image					\
  p_priority_queue				\
  p_priority_queue_fast				\
  p_priority_queue_fast_with_array		\
  p_queue					\
  p_queue_fast

category_SOURCES = category.cc
clone_SOURCES = clone.cc
exact_SOURCES = exact.cc
h_vec_SOURCES = h_vec.cc
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
tr_image_SOURCES = tr_image.cc
p_priority_queue_SOURCES = p_priority_queue.cc
p_priority_queue_fast_SOURCES = p_priority_queue_fast.cc
p_priority_queue_fast_with_array_SOURCES = p_priority_queue_fast.cc
p_queue_SOURCES = p_priority_queue_fast.cc
p_queue_fast_SOURCES = p_priority_queue_fast.cc

TESTS = $(check_PROGRAMS)
