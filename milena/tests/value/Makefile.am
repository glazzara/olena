## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

## FIXME: To be added to milena/tests/tests.mk.
full-check: check
## ...

SUBDIRS =					\
  builtin					\
  concept

#FIXME: re-enable all the tests.
check_PROGRAMS =				\
  bool						\
  equiv						\
  int_s						\
  int_s16					\
  int_u8					\
  interop					\
  label						\
  rgb8						\
  scalar					\
  set						\
  sign						\
  stack
#  float01				
#  float01_bis			
#  float01_f		
#  graylevel	
#  graylevel_f					

# FIXME: Enable when make check_full works.
# check_full_PROGRAMS =				\
#   graylevel_full				\
#   graylevel_f_full				\
#   rgb_full

bool_SOURCES = bool.cc
equiv_SOURCES = equiv.cc
#float01_SOURCES = float01.cc
#float01_bis_SOURCES = float01_bis.cc
#float01_f_SOURCES = float01_f.cc
#graylevel_SOURCES = graylevel.cc
#graylevel_f_SOURCES = graylevel_f.cc
interop_SOURCES = interop.cc
int_s_SOURCES = int_s.cc
int_s16_SOURCES = int_s16.cc
int_u8_SOURCES = int_u8.cc
label_SOURCES = label.cc
rgb8_SOURCES = rgb8.cc
scalar_SOURCES = scalar.cc
set_SOURCES = set.cc
sign_SOURCES = sign.cc
stack_SOURCES = stack.cc

# FIXME: Enable when make check_full works.
# graylevel_f_full_SOURCES = graylevel_f_full.cc
# graylevel_full_SOURCES = graylevel_full.cc
# rgb_full_SOURCES = rgb_full.cc

TESTS = $(check_PROGRAMS)
# FIXME: Enable when make check_full works.
# TESTS_FULL = $(check_full_PROGRAMS)

# FIXME: What's the purpose of this file?
EXTRA_DIST = macros.hh
