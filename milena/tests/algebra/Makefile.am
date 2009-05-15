## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  h_mat						\
  h_vec						\
  mat						\
  mat2						\
  quat						\
  vec						\
  vec2

h_mat_SOURCES = h_mat.cc
h_vec_SOURCES = h_vec.cc
mat_SOURCES = mat.cc
mat2_SOURCES = mat2.cc
quat_SOURCES = quat.cc
vec_SOURCES = vec.cc
vec2_SOURCES = vec2.cc

TESTS = $(check_PROGRAMS)
