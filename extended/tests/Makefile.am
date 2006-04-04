## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/extended -I$(top_srcdir)/metalic
# FIXME: Add 
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  id						\
  bind						\
  cast						\
  cfun						\
  cos						\
  lit						\
  math

id_SOURCES = id.cc
bind_SOURCES = bind.cc
cast_SOURCES = cast.cc
cfun_SOURCES = cfun.cc
cos_SOURCES = cos.cc
lit_SOURCES = lit.cc
math_SOURCES = math.cc

TESTS = $(check_PROGRAMS)
