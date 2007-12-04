## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  concept					\
  builtin

check_PROGRAMS =				\
  bool						\
  builtin					\
  equiv						\
  float01_bis					\
  float01					\
  float01_f					\
  graylevel					\
  interop					\
  int_s16					\
  int_s						\
  int_u8					\
  label						\
  quat						\
  rgb8						\
  scalar					\
  set

bool_SOURCES = bool.cc
builtin_SOURCES = builtin.cc
equiv_SOURCES = equiv.cc
float01_bis_SOURCES = float01_bis.cc
float01_SOURCES = float01.cc
float01_f_SOURCES = float01_f.cc
graylevel_SOURCES = graylevel.cc
interop_SOURCES = interop.cc
int_s16_SOURCES = int_s16.cc
int_s_SOURCES = int_s.cc
int_u8_SOURCES = int_u8.cc
label_SOURCES = label.cc
quat_SOURCES = quat.cc
rgb8_SOURCES = rgb8.cc
scalar_SOURCES = scalar.cc
set_SOURCES = set.cc

TESTS = $(check_PROGRAMS)
